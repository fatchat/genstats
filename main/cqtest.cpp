#include <cstdio>
#include <fstream>

#include "io/cqdata.h"

int main(int argc, char* argv[])
{
  if(argc < 2) {
    return 1;
  }
  printf("reading input file %s\n", argv[1]);
  CqData cqdata(4901, 800);
  std::ifstream ifs(argv[1]);
  const size_t nread = cqdata.readData(ifs);
  printf("read %d numbers\n", (int)nread);
}
