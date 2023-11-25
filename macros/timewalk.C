#include "qGaus.C"

TGraphErrors *
timewalk(TH2 *hin)
{
  
  auto fSignal = qTOF(100, 0., 0.05, 1.3, 1.);
  auto gWalk = new TGraphErrors;
  
  for (int ix = 0; ix < hin->GetNbinsX(); ++ix) {
    auto amp = hin->GetXaxis()->GetBinCenter(ix + 1);
    auto ampe = hin->GetXaxis()->GetBinWidth(ix + 1) / std::sqrt(12.);
    auto hpy = hin->ProjectionY("hpy", ix + 1, ix + 1);

    if (hpy->Integral() < 100) {
      delete hpy;
      continue;
    }
    
    auto maxbin = hpy->GetMaximumBin();
    auto maxcen = hpy->GetBinCenter(maxbin);
    auto maxval = hpy->GetBinContent(maxbin);
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

    auto n = gWalk->GetN();
    gWalk->SetPoint(n, amp, fSignal->GetParameter(1));
    gWalk->SetPointError(n, ampe, fSignal->GetParError(1));
    
    delete hpy;
  }
      
  return gWalk;
 
}
