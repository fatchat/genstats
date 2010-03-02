#include <cstdio>
#include <fstream>

#include "io/cqinput.h"

int main(int argc, char* argv[])
{
  if(argc < 2) {
    printf("usage: %s <input file>\n", argv[0]);
    return 1;
  }
  printf("reading input file %s\n", argv[1]);
  CqInput cqinput(argv[1]);
  if(!cqinput.valid()) {
    printf("read failed\n");
    return 1;
  }
  printf("cq dimension is %d\n", (int)cqinput.las().dim);
  printf("created %d vectors\n", (int)cqinput.las().n_vectors());

  printf("norm of element 0 is %d\n", (int)cqinput.las().norm(0));

  return 0;
}
