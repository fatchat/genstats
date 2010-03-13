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

  // count occurences of -1,0,1 in positions 0..las.dim()-1
  LinAlg::System buckets(genes->dim());
  LinAlg::Vector zero(buckets);
  LinAlg::Vector one(buckets);
  LinAlg::Vector two(buckets);

  for(size_t i = 0; i < genes->n_vectors(); ++i) {
    const LinAlg::Vector gene(i, *genes);
    for(size_t j = 0; j < genes->dim(); ++j) {
      const int val = (int)gene[j]; // cast from double
      switch(val) {
      case 0:
	++zero[j];
	break;
      case 1:
	++one[j];
	break;
      case 2:
	++two[j];
	break;
      default:
	printf("unknown entry %d at position %d in gene %d\n",
	       val, (int)j, (int)i);
	return 1;
      }
    }
  }
  
  printf("zero: "); LinAlg::print_int_vector(zero, stdout);
  printf("one: ");  LinAlg::print_int_vector(one, stdout);
  printf("two: ");  LinAlg::print_int_vector(two, stdout);

  

  return 0;
}
