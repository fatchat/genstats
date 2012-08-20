#ifndef CQINPUT_H
#define CQINPUT_H

#include <cstdio>
#include <stdexcept>
#include <fstream>
#include <vector>
#include "math/linalg.h"

namespace Cq {

// ============================= CqFile ==============================
class CqFile {

  FILE* fp_;

 public:

  CqFile   (const char*);
  ~CqFile  ();

  FILE* fp     ();
  bool  eof    () const { return feof(fp_); }

  template<class T>
    bool read_bytes(T* buffer, int nElems)
    {
      const int nread = fread ((void*)buffer, sizeof(T), nElems, fp_);

      if (eof ()) { return false; }

      if (nread != nElems)
		throw std::runtime_error("input file incomplete line");

      return true;
    }
};

// ============================= CqTextFile ==============================
class CqTextFile {

  std::ifstream ifs_;
  const size_t dim_;

 public:

 CqTextFile(const char* infile, size_t dim)
    : ifs_(infile)
    , dim_(dim)
  { }

  size_t dim() const { return dim_; }

  bool valid() const { return !!ifs_; }

  template<class T>
    size_t readline (typename std::vector<T>& dest)
    {
      const size_t len = dest.size();

      T x = 0;
      size_t i = 0;

      while (i < len) {

		ifs_ >> x;

		if (!ifs_)
		  return i;

		dest[i] = x;

		++i;
      }

      return i;
    }
};

// =========================== free functions =======================
LinAlg::System::ptr read_data(CqFile&);
LinAlg::System::ptr read_data(CqTextFile&);
} // namespace

#endif // CQINPUT_H
