#ifndef BUCKETS_H
#define BUCKETS_H

#include <cstdio>
#include "math/linalg.h"

namespace Stat {

  // ============================= Buckets ==============================
  class Buckets {

    LinAlg::System buckets_;
    bool created_;
    size_t column_total_;
  public:
    Buckets(size_t npositions, size_t nletters);
    void count(const LinAlg::VectorBase&);
    void print(size_t, FILE*);
    bool verify_sums(bool die=false);
    size_t frequency(size_t symbol, size_t position) const;
    double prob(size_t symbol, size_t position) const;
    size_t nletters() const { return buckets_.n_vectors(); }
  private:
    void create_(size_t nletters);
  };

} // namespace

#endif // BUCKETS_H
