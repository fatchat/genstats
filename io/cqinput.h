#ifndef CQINPUT_H
#define CQINPUT_H

#include "math/linalg.h"

class CqInput {
  typedef short input_type;
  typedef LinAlg::System LAS;
  LAS las_;
  bool valid_;

 public:
  CqInput(const char* infile);
  const LAS& las() const { return las_; }
  LAS& las_ref() { return las_; }
  bool valid() const { return valid_; }

 private:
  bool readData_(const char*);
};

size_t get_dim(const char*);

#endif // CQINPUT_H
