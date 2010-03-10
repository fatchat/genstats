#include <cstring>

#include "io/zlib_infdef.h"

int main(int argc, char **argv)
{
  Zlib::set_bin_mode(stdin);
  Zlib::set_bin_mode(stdout);

  if (argc == 2 && strcmp(argv[1], "-c") == 0) {
    const int ret = Zlib::def(stdin, stdout);
    if (ret)
      Zlib::zerr(ret);
    return ret;
  }

  else if (argc == 2 && strcmp(argv[1], "-d") == 0) {
    const int ret = Zlib::inf(stdin, stdout);
    if (ret)
      Zlib::zerr(ret);
    return ret;
  }
  
  else {
    fputs("zpipe usage: zpipe [-c(ompress)|-d(ecompress)] < source > destination\n", stderr);
    return 1;
  }
}
