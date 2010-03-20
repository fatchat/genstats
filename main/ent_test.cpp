#include <cstdio>
#include <exception>
#include <iostream>

#include "io/cqinput.h"
#include "math/entropy.h"
#include "math/ptree.h"
#include "util/get_opt.h"

int main(int argc, char* argv[])
{
  Getopt getopt;
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

  Stat::PTree tree;
  printf("adding vectors to tree\n");
  tree.add_vectors(*genes);
  printf("calculating probabilities\n");
  tree.calc_probs();
  printf("running show_probs\n");
  tree.show_probs(stdout);

  printf("quitting\n");
  return 0;

  const size_t nSymbols = LinAlg::count_letters(*genes);
  Stat::EntropyCalculator entropy_calc(nSymbols, genes);

  const double entropy = entropy_calc.entropy();
  printf("entropy for %d genes: %f\n", (int)genes->n_vectors(), entropy);

  const double should_be_one = entropy_calc.prob_check();
  printf("should be 1 => %f\n", should_be_one);

  return 0;
}
