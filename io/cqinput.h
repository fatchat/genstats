#ifndef CQINPUT_H
#define CQINPUT_H

#include <cstdio>
#include <stdexcept>
#include "math/linalg.h"

namespace Cq {

// ============================= CqFile ==============================
class CqFile {
  FILE* fp_;
 public:
  CqFile(const char*);
  ~CqFile();
  FILE* fp();
  bool eof() const { return feof(fp_); }
  template<class T>
    bool read_bytes(T* buffer, int nElems)
    {
      const int nread = fread((void*)buffer, sizeof(T), nElems, fp_);
      if(eof()) { return false; }
      if(nread != nElems) {
	throw std::runtime_error("input file incomplete line");
      }
      return true;
    }
};

// =========================== free functions =======================
LinAlg::System::ptr read_data(CqFile&);

} // namespace

#endif // CQINPUT_H
