#ifndef CQINPUT_H
#define CQINPUT_H

#include <cstdio>
#include "math/linalg.h"

// ============================= CqFile ==============================
class CqFile {
  FILE* fp_;
 public:
  CqFile(const char*);
  ~CqFile();
  FILE* fp();
};

// ============================= CqInput ==============================
class CqInput {

  typedef short input_type;

  int elem_sz_;
  int line_sz_;
  LinAlg::System las_;

 public:
  CqInput(CqFile&);
  const LinAlg::System& las() const { return las_; }
  LinAlg::System& las_ref() { return las_; }

 private:
  size_t get_dim_(FILE*);
  void readData_(FILE*);
};

#endif // CQINPUT_H
