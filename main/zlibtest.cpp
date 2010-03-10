#include <cstring>

#include "io/zlib_infdef.h"

/* compress or decompress from stdin to stdout */
int main(int argc, char **argv)
{
  /* avoid end-of-line conversions */
  Zlib::set_bin_mode(stdin);
  Zlib::set_bin_mode(stdout);

  /* do compression if no arguments */
  if (argc == 1) {
    const int ret = Zlib::def(stdin, stdout);
    if (ret)
      Zlib::zerr(ret);
    return ret;
  }

  /* do decompression if -d specified */
  else if (argc == 2 && strcmp(argv[1], "-d") == 0) {
    const int ret = Zlib::inf(stdin, stdout);
    if (ret)
      Zlib::zerr(ret);
    return ret;
  }
  
  /* otherwise, report usage */
  else {
    fputs("zpipe usage: zpipe [-d] < source > dest\n", stderr);
    return 1;
  }
}
