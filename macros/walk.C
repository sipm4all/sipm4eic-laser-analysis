#include "qGaus.C"

void
fun(TH2 *hin)
{
  auto g250 = new TGraph;
  auto g500 = new TGraph;
  auto g1000 = new TGraph;
  
  auto hpy = hin->ProjectionY("hpy");
  auto total = hpy->Integral();
  for (int i = 41; i < 81; ++i) {
    float val = hpy->Integral(i, i + 20);
    float win = hpy->GetBinLowEdge(i);
    g500->AddPoint(win,val / total);
    val = hpy->Integral(i, i + 10);
    g250->AddPoint(win,val / total);
    val = hpy->Integral(i, i + 40);
    g1000->AddPoint(win,val / total);
  }
  g500->Draw("alp");
  g250->Draw("samelp");
  g1000->Draw("samelp");
  hpy->DrawNormalized("same");

  auto g250_ = new TGraph;
  g250_->SetLineStyle(2);
  auto g500_ = new TGraph;
  g500_->SetLineStyle(2);
  auto g1000_ = new TGraph;
  g1000_->SetLineStyle(2);

  delete hpy;
  
  hpy = hin->ProjectionY("hpy2", 57, 57);
  hpy->SetLineStyle(2);
  total = hpy->Integral();
  for (int i = 41; i < 81; ++i) {
    float val = hpy->Integral(i, i + 20);
    float win = hpy->GetBinLowEdge(i);
    g500_->AddPoint(win,val / total);
    val = hpy->Integral(i, i + 10);
    g250_->AddPoint(win,val / total);
    val = hpy->Integral(i, i + 40);
    g1000_->AddPoint(win,val / total);
  }
  g500_->Draw("samelp");
  g250_->Draw("samelp");
  g1000_->Draw("samelp");
  hpy->DrawNormalized("same");

}

void
walk(TH2 *hin, int min_entries = 100)
{
  auto f = qTOF(100, 0., 0.1, 1.3, 1.);

  auto g1 = new TGraphErrors;
  g1->SetMarkerStyle(20);
  auto g2 = new TGraphErrors;
  g2->SetMarkerStyle(25);
  
  for (int i = 0; i < hin->GetNbinsX(); ++i) {
    auto hpy = hin->ProjectionY("hpy", i + 1, i + 1);
    if (hpy->Integral() < min_entries) {
      delete hpy;
      continue;
    }
    f->SetParameter(0, 100);
    f->SetParameter(1, 0.);
    f->SetParLimits(1, -0.5, 0.5);
    f->SetParameter(2, 0.1);
    f->SetParLimits(2, 0.01, 1.);
    f->SetParameter(3, 1.3);
    f->SetParameter(4, 1.);
    hpy->Fit(f, "0q", "", -0.5, 0.5);

    auto n = g1->GetN();
    auto amp = hin->GetXaxis()->GetBinCenter(i + 1);
    auto ampe = hin->GetXaxis()->GetBinWidth(i + 1) / std::sqrt(12.);
    g1->SetPoint(n, amp, f->GetParameter(1));
    g1->SetPointError(n, ampe, f->GetParError(1));
    g2->SetPoint(n, amp, f->GetParameter(2));
    g2->SetPointError(n, ampe, f->GetParError(2));
  }

  hin->Draw("colz");
  g1->Draw("samep");
  g2->Draw("samep");
}
