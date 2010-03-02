#ifndef CQDATA_H
#define CQDATA_H

#include <iosfwd>
#include <vector>

class CqData {

  size_t rows_;
  size_t cols_;
  std::vector<int> data_;

 public:
  CqData(size_t rows, size_t cols)
    : rows_(rows), cols_(cols) { }

  size_t readData(std::istream&, size_t = 0);
};

#endif // CQDATA_H
