#include <cstdio>
#include <fstream>
#include <algorithm>
#include <exception>

#include "io/cqinput.h"
#include "math/buckets.h"

int main(int argc, char* argv[])
{
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

  for(size_t i = 0; i < nSymbols; ++i) {
    printf("%d: ", int(i));
    buckets.print(i, stdout);
  }

  if(buckets.verify_sums()) {
    printf("sums verified\n");
  }
  else {
    printf("sums verification failed\n");
  }

  return 0;
}
