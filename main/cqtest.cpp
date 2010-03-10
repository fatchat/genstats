#include <cstdio>
#include <fstream>
#include <algorithm>
#include <exception>

#include "io/cqinput.h"

int main(int argc, char* argv[])
{
  if(argc < 2) {
    printf("usage: %s <input file>\n", argv[0]);
    return 1;
  }
  printf("reading input file %s\n", argv[1]);
  try {
    CqFile cqfile(argv[1]);
    CqInput cqinput(cqfile);
    
    printf("cq dimension is %d\n", (int)cqinput.las().dim());
    printf("created %d vectors\n", (int)cqinput.las().n_vectors());

    LinAlg::System& las = cqinput.las_ref();
    std::vector<double> norms;
    for(size_t i = 0; i < las.n_vectors(); ++i) {
      norms.push_back(LinAlg::norm(las, i));
      //    printf("%4d %3.3f\n", int(i), norms.back());
    }
  
    printf("min element is %3.3f\nmax element is %3.3f\n",
	   *std::min_element(norms.begin(), norms.end()),
	   *std::max_element(norms.begin(), norms.end()));
  }
  catch(std::exception& e) {
    printf("%s\n", e.what());
    return 1;
  }

  return 0;
}
