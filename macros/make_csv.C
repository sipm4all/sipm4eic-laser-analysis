#include "../lib/wavedesc.h"

void
make_csv(std::string dirname, int nev = kMaxInt)
{
  waveform C2(dirname + "/C2.dat");
  waveform C3(dirname + "/C3.dat");

  int iev = 0;
  while (C2.next() && C3.next() && iev < nev) {
    if (iev % 1000 == 0)
      std::cout << " --- processing event " << iev << std::endl;
    C2.get_graph().SaveAs(Form("%s/C2.%06d.txt", dirname.c_str(), iev));
    C3.get_graph().SaveAs(Form("%s/C3.%06d.txt", dirname.c_str(), iev));
    ++iev;
  }

}
