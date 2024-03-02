#include "../lib/wavedesc.h"
#include "process_signal.C"
#include "timewalk.C"

float delay = 87.7; // [ns]
float threshold = 30.; // [mV]

void
signal(std::string dirname, int nev = kMaxInt)
{
  gStyle->SetOptStat(false);
  gStyle->SetTitleXOffset(1.3);
  gStyle->SetTitleYOffset(1.3);
  
  waveform C2(dirname + "/C2.dat");
  waveform C3(dirname + "/C3.dat");

  auto hT2 = new TH1F("hT2", "", 20000, -150, 350.);
  auto hT3 = new TH1F("hT3", "", 20000, -150, 350.);

  auto hPERS = new TH2F("hPERS", ";time (ns);voltage (mV)", 2000, -15, 35., 230, -105.45570, 703.03800);
  auto hPERS_1pe = new TH2F("hPERS_1pe", ";time (ns);voltage (mV)", 2000, -15, 35., 230, -105.45570, 703.03800);
  auto hDTAMP = new TH2F("hDTAMP", ";hit amplitude (mV);hit - trigger time (ns)", 230, -105.45570, 703.03800, 200, -1.5, 3.5);
  auto hDTDVDT = new TH2F("hDTDVDT", ";hit amplitude (mV);hit - trigger time (ns)", 230 * 2, -105.45570 * 2., 703.03800 * 2., 200, -1.5, 3.5);
  auto hDTDVDT_corr = new TH2F("hDTDVDT_corr", ";hit amplitude (mV);hit - trigger time (ns)", 230 * 2, -105.45570 * 2., 703.03800 * 2., 200, -1.5, 3.5);
  auto hDTDVDT_2 = new TH2F("hDTDVDT_2", ";hit amplitude (mV);hit - trigger time (ns)", 230 * 2, -105.45570 * 2., 703.03800 * 2., 200, -1.5, 3.5);
  auto hDTAMP_corr = new TH2F("hDTAMP_corr", ";hit amplitude (mV);hit - trigger time (ns)", 230, -105.45570, 703.03800, 200, -1.5, 3.5);
  auto hDTAMP_corr2 = new TH2F("hDTAMP_corr2", ";hit amplitude (mV);hit - trigger time (ns)", 230, -105.45570, 703.03800, 200, -1.5, 3.5);
  auto hDVDT = new TH1F("hDVDT", "", 230 * 2, -105.45570 * 2, 703.03800 * 2);
  auto hDVDTAMP = new TH2F("hDVDTAMP", "", 230, -105.45570, 703.03800, 230 * 2, -105.45570 * 2, 703.03800 * 2);

  /** first processing **/
  int iev = 0;
  while (C2.next() && C3.next() && iev < nev) {
    if (iev % 1000 == 0)
      std::cout << " --- processing event " << iev << std::endl;
    ++iev;

    auto T2 = C2.get_transitions(-400., -1.);
    if (T2.size() != 1) continue;
    auto t2 = T2[0];
    hT2->Fill(t2);

    auto T3 = C3.get_transitions(threshold, 1.);
    for (auto t3 : T3) {
      hT3->Fill(t3);
      if (t3 < -0.1 || t3 > 0.3) continue;
      auto amp = C3.get_amplitude(t3 - 1.5, t3 + 3.5);
      auto delta = t3 - t2 - delay;
      hDTAMP->Fill(amp, delta);

      auto graph = C3.get_graph();
      for (int i = 0; i < graph.GetN(); ++i)
        hPERS->Fill(graph.GetX()[i], graph.GetY()[i]);

      break;
    }
  }

  /** process signal to get amplitude of 1-pe **/
  std::cout << " --- process signal " << std::endl;
  auto dataout = process_signal(hDTAMP);

  /** process again with threshold at 0.5 pe **/
  threshold = 0.5 * dataout["amp"][0];
  auto fPol1 = (TF1 *)gROOT->GetFunction("pol1");
  C2.rewind();
  C3.rewind();
  hDTAMP->Reset();
  iev = 0;
  while (C2.next() && C3.next() && iev < nev) {
    if (iev % 1000 == 0)
      std::cout << " --- processing event " << iev << std::endl;
    ++iev;
    
    auto T2 = C2.get_transitions(-400., -1.);
    if (T2.size() != 1) continue;
    auto t2 = T2[0];
    hT2->Fill(t2);

    auto T3 = C3.get_transitions(threshold, 1.);
    for (auto t3 : T3) {
      hT3->Fill(t3);
      if (t3 < -0.1 || t3 > 0.3) continue;
      auto amp = C3.get_amplitude(t3 - 1.5, t3 + 3.5);
      auto delta = t3 - t2 - delay;
      hDTAMP->Fill(amp, delta);

      auto graph = C3.get_graph();
      fPol1->SetParameter(0, 0.);
      fPol1->SetParameter(1, 100.);
      graph.Fit(fPol1, "0q", "", t3 - 0.2, t3 + 0.2);
      auto dvdt = fPol1->GetParameter(1);

      hDVDTAMP->Fill(amp, dvdt);

      if ( fabs(amp - dataout["amp"][0]) < 5 * dataout["amp-sigma"][0]) {
        hDVDT->Fill(dvdt);
        hDTDVDT->Fill(dvdt, delta);
        //        for (int i = 0; i < graph.GetN(); ++i)
        //          hPERS_1pe->Fill(graph.GetX()[i] - t3, graph.GetY()[i]);
      }

      break;
    }
  }
  
  /** make time walk and process again **/
  std::cout << " --- processing timewalk " << std::endl;
  auto gWalk = timewalk(hDTAMP);
  auto gWalk2 = timewalk(hDTDVDT);

  /** process again with time walk correction **/
  C2.rewind();
  C3.rewind();
  iev = 0;
  while (C2.next() && C3.next() && iev < nev) {
    if (iev % 1000 == 0)
      std::cout << " --- processing event " << iev << std::endl;
    ++iev;

    auto T2 = C2.get_transitions(-400., -1.);
    if (T2.size() != 1) continue;
    auto t2 = T2[0];
    hT2->Fill(t2);

    fPol1->SetParameter(0, 0.);
    fPol1->SetParameter(1, 100.);
    C2.get_graph().Fit(fPol1, "0q", "", t2 - 0.5, t2 + 0.5);
    auto t2_slope = fPol1->GetParameter(1);
    
    auto T3 = C3.get_transitions(threshold, 1.);
    for (auto t3 : T3) {
      hT3->Fill(t3);
      if (t3 < -0.1 || t3 > 0.3) continue;
      auto amp = C3.get_amplitude(t3 - 1.5, t3 + 3.5);
      auto delta = t3 - t2 - delay - gWalk->Eval(amp);
      hDTAMP_corr->Fill(amp, delta);

      auto graph = C3.get_graph();
      fPol1->SetParameter(0, 0.);
      fPol1->SetParameter(1, 100.);
      graph.Fit(fPol1, "0q", "", t3 - 0.2, t3 + 0.2);
      auto dvdt = fPol1->GetParameter(1);
      auto delta2 = t3 - t2 - delay - gWalk2->Eval(dvdt);
      hDTAMP_corr2->Fill(amp, delta2);

      if ( fabs(amp - dataout["amp"][0]) < 5 * dataout["amp-sigma"][0] &&
           delta > -1.5 && delta < 3.5) {
        hDVDT->Fill(dvdt);
        hDTDVDT_corr->Fill(dvdt, delta2);
        hDTDVDT_2->Fill(-t2_slope, delta2);
        for (int i = 0; i < graph.GetN(); ++i)
          hPERS_1pe->Fill(graph.GetX()[i] + delta, graph.GetY()[i]);
      }

      
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
  hDTAMP->Write();
  hDTAMP_corr->Write();
  hDTAMP_corr2->Write();
  gWalk->Write("gWalk");
  gWalk2->Write("gWalk2");
  hPERS_1pe->Write();
  hDTDVDT->Write();
  hDTDVDT_corr->Write();
  hDTDVDT_2->Write();
  hDVDT->Write();
  hDVDTAMP->Write();
  fout->Close();
  
}
