#include "../lib/wavedesc.h"

const float delay = 87.7;

void
signal_pde(std::string dirname, int nev = kMaxInt)
{
  //  gStyle->SetOptStat(false);
  gStyle->SetTitleXOffset(1.3);
  gStyle->SetTitleYOffset(1.3);
  
  waveform C2(dirname + "/C2.dat");
  waveform C3(dirname + "/C3.dat");

  auto hT2 = new TH1F("hT2", "", 20000, -150, 350.);
  auto hT3 = new TH1F("hT3", "", 20000, -150, 350.);

  auto hPERS = new TH2F("hPERS", ";time (ns);voltage (mV)", 2000, -15. + delay, 35. + delay, 230, -105.45570, 703.03800);
  auto hDTAMP = new TH2F("hDTAMP", ";hit amplitude (mV);hit - trigger time (ns)", 230, -105.45570, 703.03800, 200, -1.5, 3.5);
  
  int iev = 0;
  while (C2.next() && C3.next() && iev < nev) {
    if (iev % 1000 == 0)
      std::cout << " --- processing event " << iev << std::endl;
    ++iev;

    auto T2 = C2.get_transitions(-400., -1.);
    if (T2.size() != 1) continue;
    auto t2 = T2[0];
    hT2->Fill(t2);

    auto T3 = C3.get_transitions(25., 1.);
    for (auto t3 : T3) {
      hT3->Fill(t3);
      //      if (t3 < -0.1 || t3 > 0.3) continue;
      auto delta = t3 - t2 - delay;
      auto amp = C3.get_amplitude(t3 - 1.5, t3 + 3.5);
      hDTAMP->Fill(amp, delta);

      auto graph = C3.get_graph();
      for (int i = 0; i < graph.GetN(); ++i)
        hPERS->Fill(graph.GetX()[i], graph.GetY()[i]);
      break;
    }
  }

  std::string pngfilename = dirname + "/signal.png";
  auto cSignal = new TCanvas("cSignal", "cSignal", 800, 800);
  cSignal->Divide(2, 2);
  cSignal->cd(1)->SetLogz();
  hPERS->Draw("colz");
  cSignal->cd(2);
  hDTAMP->Draw("colz");
  cSignal->cd(3);
  auto hpy = hDTAMP->ProjectionY();
  hpy->SetLineWidth(2);
  hpy->SetLineColor(kAzure-3);
  hpy->SetFillStyle(3001);
  hpy->SetFillColor(kAzure-3);  
  hpy->Draw();
  cSignal->cd(4);
  auto hpx = hDTAMP->ProjectionX();
  hpx->SetLineWidth(2);
  hpx->SetLineColor(kAzure-3);
  hpx->SetFillStyle(3001);
  hpx->SetFillColor(kAzure-3);  
  hpx->Draw();
  cSignal->SaveAs(pngfilename.c_str());
  
  std::string outfilename = dirname + "/signal.root";
  auto fout = TFile::Open(outfilename.c_str(), "RECREATE");
  hT2->Write();
  hDTAMP->Write();
  fout->Close();
  
}
