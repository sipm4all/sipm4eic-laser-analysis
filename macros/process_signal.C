#include "qGaus.C"

std::map<std::string, std::array<double, 2>>
process_signal(TH2 *hin)
{
  std::map<std::string, std::array<double, 2>> dataout;

  auto fGaus = (TF1 *)gROOT->GetFunction("gaus");
  auto fSignal = (TF1 *)gROOT->GetFunction("qTOF");
  if (!fSignal) fSignal = qTOF(100, 0., 0.05, 1.3, 1.);

  auto deltamin = hin->GetYaxis()->FindBin(-1.5 + 0.001);
  auto deltamax = hin->GetYaxis()->FindBin(3.5 - 0.001);
  auto hpx = hin->ProjectionX("hpx", deltamin, deltamax);
  if (hpx->Integral() < 100) {
    delete hpx;
    return dataout;
  }

  /** HACK **/
  hpx->SetBinContent(213, 0.);
  hpx->SetBinError(213, 0.);
  
  /** amplitude **/
  
  auto maxbin = hpx->GetMaximumBin();
  auto maxcen = hpx->GetBinCenter(maxbin);
  auto maxval = hpx->GetBinContent(maxbin);
  fGaus->SetParameter(0, maxval);
  fGaus->SetParameter(1, maxcen);
  fGaus->SetParameter(2, 5.);
  hpx->Fit(fGaus, "0q", "", maxcen - 25., maxcen + 25.);
  auto amp = fGaus->GetParameter(1);
  auto ampe = fGaus->GetParError(1);
  auto amps = fGaus->GetParameter(2);
  auto ampse = fGaus->GetParError(2);

  /** cross talk **/
  
  auto spe = fGaus->Integral(amp - 5. * amps, amp + 5. * amps) / hpx->GetBinWidth(1);
  auto spee = fGaus->IntegralError(amp - 5. * amps, amp + 5. * amps) / hpx->GetBinWidth(1);
  auto xtalk = (hpx->GetEntries() - spe) / hpx->GetEntries();
  auto xtalke = spee / hpx->GetEntries();
  
  dataout["amp"] = {amp, ampe};
  dataout["amp-sigma"] = {amps, ampse};
  dataout["xtalk"] = {xtalk, xtalke};
  
  /** time resolution **/

  auto ampmin = hpx->FindBin(amp - 3. * amps);
  auto ampmax = hpx->FindBin(amp + 3. * amps);
  auto hpy = hin->ProjectionY("hpy", ampmin, ampmax);
  
  if (hpy->Integral() < 1000) {
    delete hpx;
    delete hpy;
    return dataout;
  }
  
  maxbin = hpy->GetMaximumBin();
  maxcen = hpy->GetBinCenter(maxbin);
  maxval = hpy->GetBinContent(maxbin);
  fSignal->SetParameter(0, maxval);
  fSignal->SetParameter(1, maxcen);
  fSignal->SetParameter(2, 0.1);
  fSignal->SetParameter(3, 1.3);
  fSignal->SetParameter(4, 1.);
  auto fitmin = fSignal->GetParameter(1) - 3. * fSignal->GetParameter(2);
  auto fitmax = fSignal->GetParameter(1) + 3. * fSignal->GetParameter(2);
  hpy->Fit(fSignal, "0q", "", fitmin, fitmax);
  fitmin = fSignal->GetParameter(1) - 3. * fSignal->GetParameter(2);
  fitmax = fSignal->GetParameter(1) + 3. * fSignal->GetParameter(2);
  hpy->Fit(fSignal, "0q", "", fitmin, fitmax);
  
  auto fitpeak = fSignal->GetParameter(0);
  auto fitpeake = fSignal->GetParError(0);
  
  auto bkglev = 0.;
  
  auto fwhm_lo = fSignal->GetX(0.5 * (fitpeak - bkglev) + bkglev, -1, fSignal->GetParameter(1));
  auto fwhm_hi = fSignal->GetX(0.5 * (fitpeak - bkglev) + bkglev, fSignal->GetParameter(1), 1.);
  auto fwhm = fwhm_hi - fwhm_lo;
  
  fwhm_lo = fSignal->GetX(0.5 * (fitpeak + fitpeake - bkglev) + bkglev, -1, fSignal->GetParameter(1));
  fwhm_hi = fSignal->GetX(0.5 * (fitpeak + fitpeake - bkglev) + bkglev, fSignal->GetParameter(1), 1.);
  auto fwhme = std::fabs(fwhm - (fwhm_hi - fwhm_lo));
  
  dataout["fwhm"] = {fwhm, fwhme};

  return dataout;
}

