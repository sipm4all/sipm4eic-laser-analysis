#include "../lib/wavedesc.h"

void
make_csv(std::string dirname, int nev = kMaxInt)
{
  waveform C2(dirname + "/C2.dat");
  waveform C3(dirname + "/C3.dat");

  int nC2;
  float xC2[1024*1024];
  float yC2[1024*1024];
  auto tC2 = new TTree("tC2", "tC2");
  tC2->Branch("n", &nC2, "n/I");
  tC2->Branch("x", &xC2, "x[n]/F");
  tC2->Branch("y", &yC2, "y[n]/F");
  
  int nC3;
  float xC3[1024*1024];
  float yC3[1024*1024];
  auto tC3 = new TTree("tC3", "tC3");
  tC3->Branch("n", &nC3, "n/I");
  tC3->Branch("x", &xC3, "x[n]/F");
  tC3->Branch("y", &yC3, "y[n]/F");
  
  
  int iev = 0;
  while (C2.next() && C3.next() && iev < nev) {
    if (iev % 1000 == 0)
      std::cout << " --- processing event " << iev << std::endl;
    
    auto C2 = C2.get_graph();
    auto C3 = C3.get_graph();
  }

}
