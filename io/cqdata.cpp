#include "io/cqdata.h"

#include <iostream>
#include <fstream>

size_t CqData::readData(std::istream& is, size_t nrows_to_read)
{
  if(!nrows_to_read) {
    nrows_to_read = rows_;
  }
  data_.resize(nrows_to_read * cols_);
  
  int d = 0;
  size_t offset = 0;
  // change to use mmap asap
  while (offset <= nrows_to_read * cols_) {
    is >> d;
    if (is) {
      data_[offset] = d;
      ++offset;
    }
    else {
      break;
    }
  }
  return offset;
}
