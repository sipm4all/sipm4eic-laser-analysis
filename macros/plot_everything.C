#include "process_signal.C"
#include "style.C"

std::vector<std::string> vbiases_s13 = {
  "50.0", "50.5", "51.0", "51.5", "52.0", "52.5", "53.0", "54.0", "55.0", "57.0"
};

std::vector<std::string> vbiases_s14 = {
  "38.5", "39.0", "39.5", "40.0", "40.5", "41.0", "41.5", "42.5", "43.5", "45.5"
};

std::map<std::string, TGraphErrors *> process_data(std::string dirname, std::vector<std::string> vbiases, float vbreak = 0., std::string what = "hDTAMP_corr");
void draw(std::string what);

void gstyle(TGraph *g, int m, int c, const char *title = "")
{
  g->SetLineColor(c);
  g->SetMarkerColor(c);
  g->SetMarkerStyle(m);
  g->SetMarkerSize(1.2);
  g->SetLineWidth(1);
  g->SetFillStyle(0);
  g->SetTitle(title);
}

void fwhm_amp_plot()
{
  style();
  auto c = new TCanvas("c", "c", 800, 800);
  c->DrawFrame(0., 0.1, 600., 0.3, ";amplitude (mV); FWHM (ns)");
  draw("fwhm-amp");
}

void amp_vbias_plot()
{
  style();
  auto c = new TCanvas("c", "c", 800, 800);
  c->DrawFrame(35., 0., 60., 500., ";bias voltage (V);amplitude (mV)");
  draw("amp-vbias");
}

void draw(std::string what)
{
  auto gA1_150 = process_data("../data/sptr/20231121-113131/", vbiases_s13)[what];
  gstyle(gA1_150, 20, kAzure-3, "S13360-3050");
  auto gB1_150 = process_data("../data/sptr/20231121-215404/", vbiases_s13)[what];
  gstyle(gB1_150, 20, kRed+1, "S13360-3075");
  auto gC1_150 = process_data("../data/sptr/20231122-092935/", vbiases_s14)[what];
  gstyle(gC1_150, 20, kGreen+2, "S14160-3050");
  auto gPP_150 = process_data("../data/sptr/20231122-153233/", vbiases_s13)[what];
  gstyle(gPP_150, 25, kAzure-3, "proto-3050");

  gA1_150->Draw("samelp");
  gB1_150->Draw("samelp");
  gC1_150->Draw("samelp");
  gPP_150->Draw("samelp");

  auto gA1_175 = process_data("../data/sptr/20231123-133152/", vbiases_s13)[what];
  gstyle(gA1_175, 20, kAzure-3, "S13360-3050");
  auto gB1_175 = process_data("../data/sptr/20231123-204658/", vbiases_s13)[what];
  gstyle(gB1_175, 20, kRed+1, "S13360-3075");
  auto gC1_175 = process_data("../data/sptr/20231124-111410/", vbiases_s14)[what];
  gstyle(gC1_175, 20, kGreen+2, "S14160-3050");
  auto gPP_175 = process_data("../data/sptr/20231124-192220/", vbiases_s13)[what];
  gstyle(gPP_175, 25, kRed+1, "proto-3075");

  gA1_175->Draw("samelp");
  gB1_175->Draw("samelp");
  gC1_175->Draw("samelp");
  gPP_175->Draw("samelp");

}

void resolution_plot()
{
  style();
  
  auto c = new TCanvas("c", "c", 800, 800);
  c->DrawFrame(1., 0.125, 10., 0.25, ";over voltage (V); FWHM (ns)");

  auto gA1_150 = process_data("../data/sptr/20231121-113131/", vbiases_s13, 48.25)["fwhm"];
  gstyle(gA1_150, 20, kAzure-3, "S13360-3050");
  auto gB1_150 = process_data("../data/sptr/20231121-215404/", vbiases_s13, 48.0)["fwhm"];
  gstyle(gB1_150, 20, kRed+1, "S13360-3075");
  auto gC1_150 = process_data("../data/sptr/20231122-092935/", vbiases_s14, 36.3)["fwhm"];
  gstyle(gC1_150, 20, kGreen+2, "S14160-3050");
  auto gPP_150 = process_data("../data/sptr/20231122-153233/", vbiases_s13, 49.1)["fwhm"];
  gstyle(gPP_150, 25, kAzure-3, "proto-3050");

  gA1_150->Draw("samelp");
  gB1_150->Draw("samelp");
  gC1_150->Draw("samelp");
  gPP_150->Draw("samelp");

  auto gA1_175 = process_data("../data/sptr/20231123-133152/", vbiases_s13, 48.25)["fwhm"];
  gstyle(gA1_175, 20, kAzure-3, "S13360-3050");
  auto gB1_175 = process_data("../data/sptr/20231123-204658/", vbiases_s13, 48.0)["fwhm"];
  gstyle(gB1_175, 20, kRed+1, "S13360-3075");
  auto gC1_175 = process_data("../data/sptr/20231124-111410/", vbiases_s14, 36.3)["fwhm"];
  gstyle(gC1_175, 20, kGreen+2, "S14160-3050");
  auto gPP_175 = process_data("../data/sptr/20231124-192220/", vbiases_s13, 49.3)["fwhm"];
  gstyle(gPP_175, 25, kRed+1, "proto-3075");

  gA1_175->Draw("samelp");
  gB1_175->Draw("samelp");
  gC1_175->Draw("samelp");
  gPP_175->Draw("samelp");

}

void amplitude_plot()
{
  style();
  
  auto c = new TCanvas("c", "c", 800, 800);
  c->DrawFrame(0., 0., 100., 800., ";bias voltage (V); amplitude (mV)");

  auto gA1_150 = process_data("../data/sptr/20231121-113131/", vbiases_s13, 0.)["amp"];
  gstyle(gA1_150, 20, kAzure-3, "S13360-3050");
  auto gB1_150 = process_data("../data/sptr/20231121-215404/", vbiases_s13, 0.)["amp"];
  gstyle(gB1_150, 20, kRed+1, "S13360-3075");
  auto gC1_150 = process_data("../data/sptr/20231122-092935/", vbiases_s14, 0.)["amp"];
  gstyle(gC1_150, 20, kGreen+2, "S14160-3050");
  auto gPP_150 = process_data("../data/sptr/20231122-153233/", vbiases_s13, 0.)["amp"];
  gstyle(gPP_150, 25, kAzure-3, "proto-3050");

  gA1_150->Draw("samelp");
  gB1_150->Draw("samelp");
  gC1_150->Draw("samelp");
  gPP_150->Draw("samelp");

  auto gA1_175 = process_data("../data/sptr/20231123-133152/", vbiases_s13, 0.)["amp"];
  gstyle(gA1_175, 20, kAzure-3, "S13360-3050");
  auto gB1_175 = process_data("../data/sptr/20231123-204658/", vbiases_s13, 0.)["amp"];
  gstyle(gB1_175, 20, kRed+1, "S13360-3075");
  auto gC1_175 = process_data("../data/sptr/20231124-111410/", vbiases_s14, 0.)["amp"];
  gstyle(gC1_175, 20, kGreen+2, "S14160-3050");
  auto gPP_175 = process_data("../data/sptr/20231124-192220/", vbiases_s13, 0.)["amp"];
  gstyle(gPP_175, 25, kRed+1, "proto-3075");

  gA1_175->Draw("samelp");
  gB1_175->Draw("samelp");
  gC1_175->Draw("samelp");
  gPP_175->Draw("samelp");

}

std::map<std::string, TGraphErrors *>
process_data(std::string dirname, std::vector<std::string> vbiases, float vbreak = 0., std::string what)
{

  auto gReso = new TGraphErrors;
  auto gAmpV = new TGraphErrors;
  auto gXtalk = new TGraphErrors;
  auto gResoAmp = new TGraphErrors;
  auto gDVDT = new TGraphErrors;
  auto gSigmaEle = new TGraphErrors;
  
  for (int i = 0; i < vbiases.size(); ++i) {
    auto &vbias = vbiases[i];
    //    std::cout << " --- doing Vbias " << vbias << std::endl;
    float v = std::atof(vbias.c_str());
    //    std::string filename = dirname + "/" + vbias + "v.on" + "/signal.root";
    std::string filename = dirname + "/" + vbias + "V" + "/signal.root";
    auto fin = TFile::Open(filename.c_str());
    auto hin = (TH2 *)fin->Get(what.c_str());
    auto dataout = process_signal(hin);

    auto hDVDT = (TH1 *)fin->Get("hDVDT");
    auto hPERS = (TH2 *)fin->Get("hPERS_1pe");
    auto persmin = hPERS->GetXaxis()->FindBin(-10. + 0.001);
    auto persmax = hPERS->GetXaxis()->FindBin(-5. - 0.001);
    auto hNoise = hPERS->ProjectionY("hNoise", persmin, persmax);
    if (hDVDT->Integral() > 100) {
      auto fGaus = (TF1 *)gROOT->GetFunction("gaus");
      hNoise->Fit(fGaus, "0q", "");
      auto noise = fGaus->GetParameter(2);
      
      auto maxbin = hDVDT->GetMaximumBin();
      auto maxcen = hDVDT->GetBinCenter(maxbin);
      auto maxval = hDVDT->GetBinContent(maxbin);
      fGaus->SetParameter(0, maxval);
      fGaus->SetParameter(1, maxcen);
      fGaus->SetParameter(2, 10.);
      hDVDT->Fit(fGaus, "0q", "");
      auto n = gSigmaEle->GetN();
      gDVDT->SetPoint(n, v, fGaus->GetParameter(1));
      gDVDT->SetPointError(n, 0., fGaus->GetParError(1));
      gSigmaEle->SetPoint(n, v, 2.35 * noise / fGaus->GetParameter(1));
      gSigmaEle->SetPointError(n, 0., 0.);
    }
    delete hNoise;
    
    if (dataout.count("amp")) {
      auto n = gAmpV->GetN();
      gAmpV->SetPoint(n, v, dataout["amp"][0]);
      gAmpV->SetPointError(n, 0., dataout["amp"][1]);
    }

    if (dataout.count("xtalk")) {
      auto n = gXtalk->GetN();
      gXtalk->SetPoint(n, v - vbreak, dataout["xtalk"][0]);
      gXtalk->SetPointError(n, 0., dataout["xtalk"][1]);
    }

    if (dataout.count("fwhm")) {
      auto n = gReso->GetN();
      gReso->SetPoint(n, v - vbreak, dataout["fwhm"][0]);
      gReso->SetPointError(n, 0, dataout["fwhm"][1]);
    }
    
    if (dataout.count("amp") && dataout.count("fwhm")) {
      auto n = gResoAmp->GetN();
      gResoAmp->SetPoint(n, dataout["amp"][0], dataout["fwhm"][0]);
      gResoAmp->SetPointError(n, dataout["amp"][1], dataout["fwhm"][1]);
    }

  }
      
  return { {"fwhm", gReso} , {"xtalk", gXtalk} , {"amp-vbias", gAmpV} , {"fwhm-amp", gResoAmp} , {"sigma-ele", gSigmaEle} , {"dvdt", gDVDT} };
 
}
