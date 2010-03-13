#include <cstdio>
#include <fstream>
#include <algorithm>
#include <exception>

#include "io/cqinput.h"

int main(int argc, char* argv[])
{
  if(argc < 2 || argc > 3) {
    printf("usage: %s <input bin file>\n", argv[0]);
    printf("usage: %s <input text file> <dim>\n", argv[0]);
    return 1;
  }
  printf("reading input file %s\n", argv[1]);
  LinAlg::System::ptr las;
  try {
    if(argc == 2) {
      Cq::CqFile cqfile(argv[1]);
      las = Cq::read_data(cqfile);
    }
    else {
      const size_t dim = (size_t)atoi(argv[2]);
      Cq::CqTextFile cqtextfile(argv[1], dim);
      las = Cq::read_data(cqtextfile);
    }
  }
  catch(std::exception& e) {
    printf("%s\n", e.what());
    return 1;
  }

  std::vector<double> norms;
  for(size_t i = 0; i < las->n_vectors(); ++i) {
    norms.push_back(LinAlg::norm(*las, i));
    //    printf("%4d %3.3f\n", int(i), norms.back());
  }
  
  printf("min element is %3.3f\nmax element is %3.3f\n",
	 *std::min_element(norms.begin(), norms.end()),
	 *std::max_element(norms.begin(), norms.end()));

  return 0;
}
