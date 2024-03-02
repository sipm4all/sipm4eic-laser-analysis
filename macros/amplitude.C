#include "process_signal.C"
#include "style.C"

std::vector<std::string> vbiases_s13 = {
  "50.0", "50.5", "51.0", "51.5", "52.0", "52.5", "53.0", "54.0", "55.0", "57.0"
};

std::vector<std::string> vbiases_s14 = {
  "38.5", "39.0", "39.5", "40.0", "40.5", "41.0", "41.5", "42.5", "43.5", "45.5"
};

TGraphErrors *resolution(std::string dirname, std::vector<std::string> vbiases, float vbreak = 0., std::string what = "hDTAMP_corr");

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

void plotme()
{
  style();
  
  auto c = new TCanvas("c", "c", 800, 800);
  c->DrawFrame(1., 0.125, 10., 0.25, ";over voltage (V); FWHM (ns)");

  auto gA1_150 = resolution("../data/sptr/20231121-113131/", vbiases_s13, 48.25);
  gstyle(gA1_150, 20, kAzure-3, "S13360-3050");
  auto gB1_150 = resolution("../data/sptr/20231121-215404/", vbiases_s13, 48.0);
  gstyle(gB1_150, 20, kRed+1, "S13360-3075");
  auto gC1_150 = resolution("../data/sptr/20231122-092935/", vbiases_s14, 36.3);
  gstyle(gC1_150, 20, kGreen+2, "S14160-3050");
  auto gPP_150 = resolution("../data/sptr/20231122-153233/", vbiases_s13, 49.1);
  gstyle(gPP_150, 25, kAzure-3, "proto-3050");

  gA1_150->Draw("samelp");
  gB1_150->Draw("samelp");
  gC1_150->Draw("samelp");
  gPP_150->Draw("samelp");

  auto gA1_175 = resolution("../data/sptr/20231123-133152/", vbiases_s13, 48.25);
  gstyle(gA1_175, 20, kAzure-3, "S13360-3050");
  auto gB1_175 = resolution("../data/sptr/20231123-204658/", vbiases_s13, 48.0);
  gstyle(gB1_175, 20, kRed+1, "S13360-3075");
  auto gC1_175 = resolution("../data/sptr/20231124-111410/", vbiases_s14, 36.3);
  gstyle(gC1_175, 20, kGreen+2, "S14160-3050");
  auto gPP_175 = resolution("../data/sptr/20231124-192220/", vbiases_s13, 49.1);
  gstyle(gPP_175, 25, kRed+1, "proto-3075");

  gA1_175->Draw("samelp");
  gB1_175->Draw("samelp");
  gC1_175->Draw("samelp");
  gPP_175->Draw("samelp");

}

TGraphErrors *
amplitude(std::string dirname, std::vector<std::string> vbiases, float vbreak = 0., std::string what)
{
  auto gAmp = new TGraphErrors; 
  for (int i = 0; i < vbiases.size(); ++i) {
    auto &vbias = vbiases[i];
    float v = std::atof(vbias.c_str());
    std::string filename = dirname + "/" + vbias + "v.on" + "/signal.root";
    auto fin = TFile::Open(filename.c_str());
    auto hin = (TH2 *)fin->Get(what.c_str());
    auto dataout = process_signal(hin);
    if (!dataout.count("amp")) continue;
    auto n = gAmp->GetN();
    gAmp->SetPoint(n, v - vbreak, dataout["amp"][0]);
    gAmp->SetPointError(n, 0., dataout["amp"][1]);
  }      
  return gAmp; 
}
