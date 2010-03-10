#ifndef ZLIB_INFDEF_H
#define ZLIB_INFDEF_H

#include <cstdio>

namespace Zlib {

  int def(FILE *source, FILE *dest);
  int inf(FILE *source, FILE *dest);
  void zerr(int);
  void set_bin_mode(FILE*);

} // namespace

#endif // ZLIB_INFDEF_H
