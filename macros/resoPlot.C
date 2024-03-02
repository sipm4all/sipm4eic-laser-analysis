#include "qGaus.C"
#include "emg.C"

TGraphErrors *
resoPlot(const char *tag, std::vector<std::string> vbiases = {"51", "52", "53", "55", "57"})
{


  auto fPol0 = (TF1 *)gROOT->GetFunction("pol0");
  auto fGaus = (TF1 *)gROOT->GetFunction("gaus");
  auto fSignal = qTOF(100, 0., 0.05, 10.);
  //  fSignal = EMG(1, 0., 0.1, 100., -10, 10.);
  
  auto gAmp = new TGraphErrors;
  auto gReso = new TGraphErrors;
  
  for (int i = 0; i < vbiases.size(); ++i) {
    auto &vbias = vbiases[i];
    float v = std::atof(vbias.c_str());
    auto fin = TFile::Open(Form("/home/preghenella/EIC/laser/OSCILLOSCOPIO/Roberto/data/%s/%iV/signal.root", tag, std::atoi(vbias.c_str())));
    if (!fin || !fin->IsOpen()) continue;
    auto hin = (TH2 *)fin->Get("hDTAMP");

    auto hpx = hin->ProjectionX("hpx");
    hpx->SetBinContent(213, 0.);
    hpx->SetBinError(213, 0.);
    
    auto maxbin = hpx->GetMaximumBin();
    auto maxcen = hpx->GetBinCenter(maxbin);
    auto maxval = hpx->GetBinContent(maxbin);
    fGaus->SetParameter(0, maxval);
    fGaus->SetParameter(1, maxcen);
    fGaus->SetParameter(2, 5.);
    hpx->Fit(fGaus, "", "", maxcen - 25., maxcen + 25.);

    gAmp->SetPoint(i, v, fGaus->GetParameter(1));
    gAmp->SetPointError(i, 0., fGaus->GetParError(1));

    auto ampmin = hpx->FindBin(fGaus->GetParameter(1) - 3. * fGaus->GetParameter(2));
    auto ampmax = hpx->FindBin(fGaus->GetParameter(1) + 3. * fGaus->GetParameter(2));
    auto hpy = hin->ProjectionY("hpy", ampmin, ampmax);

    auto bkgmin = hpy->FindBin(1.5 + 0.001);
    auto bkgmax = hpy->FindBin(3.5 - 0.001);
    auto bkgsum = hpy->Integral(bkgmin, bkgmax);
    auto bkglev = bkgsum / (bkgmax - bkgmin + 1);
    
    fSignal->SetParameter(0, 1000);
    fSignal->SetParameter(1, 0.);
    fSignal->SetParameter(2, 0.05);
    fSignal->SetParameter(3, 1.3);
    fSignal->SetParameter(4, 1.);
    hpy->Fit(fSignal, "0q");

    auto fitmin = fSignal->GetParameter(1) - 0.2;
    auto fitmax = fSignal->GetParameter(1) + 0.2;
    hpy->Fit(fSignal, "", "", fitmin, fitmax);
    fitmin = fSignal->GetParameter(1) - 0.2;
    fitmax = fSignal->GetParameter(1) + 0.2;
    //    hpy->Fit(fSignal, "", "", fitmin, fitmax);

    //    return nullptr;
    
    auto fitpeak = fSignal->GetParameter(0);
    auto fitpeake = fSignal->GetParError(0);
    std::cout << fitpeak << " " << fitpeake << std::endl;
    
    auto fwhm_lo = fSignal->GetX(0.5 * (fitpeak - bkglev) + bkglev, -1, fSignal->GetParameter(1));
    auto fwhm_hi = fSignal->GetX(0.5 * (fitpeak - bkglev) + bkglev, fSignal->GetParameter(1), 1.);
    std::cout << fwhm_lo << " " << fwhm_hi << std::endl;
    auto fwhm = fwhm_hi - fwhm_lo;

    fwhm_lo = fSignal->GetX(0.5 * (fitpeak + fitpeake - bkglev) + bkglev, -1, fSignal->GetParameter(1));
    fwhm_hi = fSignal->GetX(0.5 * (fitpeak + fitpeake - bkglev) + bkglev, fSignal->GetParameter(1), 1.);
    std::cout << fwhm_lo << " " << fwhm_hi << std::endl;
    auto fwhme = std::fabs(fwhm - (fwhm_hi - fwhm_lo));

    auto n = gReso->GetN();
    gReso->SetPoint(n, v, fwhm);
    gReso->SetPointError(n, 0, fwhme);
    
  }

  //  gReso->Draw("alp");
  //  gAmp->Draw("alp");
  return gReso;
}
