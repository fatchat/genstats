#include <cstdio>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <exception>
#include <iostream>

#include "io/cqinput.h"
#include "math/buckets.h"
#include "math/entropy.h"
#include "util/get_opt.h"

int main(int argc, char* argv[])
{
  Getopt getopt;
  getopt.addOption("history", Option::NO_ARG);
  getopt.addOption("probs", Option::NO_ARG);
  getopt.addToHelp(" <input file>");
  if (getopt.processOpts(argc, argv)) {
    getopt.showHelp(std::cout);
    exit(1);
  }

  if(argc - getopt.first_non_opt() != 1) {
    getopt.showHelp(std::cout);
    exit(1);
  }

  const char* filename = argv[getopt.first_non_opt()];
  const bool show_hist = getopt.getOption('h').is_set();
  const bool show_probs = getopt.getOption('p').is_set();

  // read data, create System pointer
  printf("reading input file %s\n", filename);
  LinAlg::System::ptr genes;
  try {
    Cq::CqFile cqfile(filename);
    genes = Cq::read_data(cqfile);
  }
  catch(std::exception& e) {
    printf("%s\n", e.what());
    return 1;
  }

  // count occurences of 0,1,2 in different gene positions
  const size_t nSymbols = 3;
  Stat::Buckets buckets(genes->dim(), nSymbols);

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

  buckets.verify_sums(true);

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

  Stat::EntropyCalculator entropy_calc(nSymbols, genes);
  const double entropy = entropy_calc.entropy();
  printf("entropy for %d genes: %f\n", (int)genes->n_vectors(), entropy);
  const double should_be_one = entropy_calc.prob_check();
  printf("should be 1 => %f\n", should_be_one);
  return 0;
}
