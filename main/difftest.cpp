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
  LinAlg::System::ptr las;
  try {
    Cq::CqFile cqfile(filename);
    las = Cq::read_data(cqfile);
  }
  catch(std::exception& e) {
    printf("%s\n", e.what());
    return 1;
  }

  if(!las->n_vectors()) {
    printf("No vectors in system!\n");
    return 1;
  }

  const size_t n_vectors = las->n_vectors();
  for(size_t i = 0; i < n_vectors; ++i) {
    size_t diff = las->difference(0, i);
    LinAlg::ConstVector cv (diff, *las);
    printf("%3d: ", (int)i); print_int_vector(cv, stdout);
  }

  return 0;
}
