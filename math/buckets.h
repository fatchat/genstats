#ifndef BUCKETS_H
#define BUCKETS_H

#include <cstdio>
#include "math/linalg.h"

namespace Stat {

  // ============================= Buckets ==============================
  class Buckets {

    LinAlg::System buckets_;
    bool created_;
  public:
    Buckets(size_t dim);
    void create(size_t n);
    void count(const LinAlg::Vector&);
    void print(size_t, FILE*);
    bool verify_sums() const;
  };

} // namespace

#endif // BUCKETS_H
