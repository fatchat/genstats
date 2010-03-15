#include <cstdio>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <exception>

#include "io/cqinput.h"
#include "math/buckets.h"

int main(int argc, char* argv[])
{
  bool show_hist = false;
  bool show_probs = true;
  if(argc < 2) {
    printf("usage: %s <input file>\n", argv[0]);
    return 1;
  }
  // read data, create System pointer
  printf("reading input file %s\n", argv[1]);
  LinAlg::System::ptr genes;
  try {
    Cq::CqFile cqfile(argv[1]);
    genes = Cq::read_data(cqfile);
  }
  catch(std::exception& e) {
    printf("%s\n", e.what());
    return 1;
  }

  // count occurences of 0,1,2 in different gene positions
  Stat::Buckets buckets(genes->dim());
  const size_t nSymbols = 3;
  buckets.create(nSymbols);

  for(size_t i = 0; i < genes->n_vectors(); ++i) {
    const LinAlg::Vector gene(i, *genes);
    buckets.count(gene);
  }

  if(show_hist) {
  for(size_t i = 0; i < nSymbols; ++i) {
    printf("%d: ", int(i));
    buckets.print(i, stdout);
  }
  }

  assert(buckets.verify_sums());

  if(show_probs) {
    for(size_t position = 0; position < genes->dim(); ++position) {
      double psum = 0;
      for(size_t symbol = 0; symbol < nSymbols; ++symbol) {
	const double prob = buckets.prob(symbol, position);
	printf("P[X_%d = %d] = %1.3f\n", (int)position, (int)symbol, prob);
	psum += prob;
      }
      assert(fabs(psum - 1.0) < 1e-10);
      printf("\n");
    }
  }

  return 0;
}
