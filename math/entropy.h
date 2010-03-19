#ifndef ENTROPY_H
#define ENTROPY_H

#include <vector>

#include "math/linalg.h"
#include "math/buckets.h"

namespace Stat {

  class EntropyCalculator {
    LinAlg::System::ptr vectors_;
    Buckets buckets_;

  public:
    EntropyCalculator(size_t nletters, LinAlg::System::ptr vectors);
    double entropy() const;
    double prob_check() const;

  private:
    double vector_prob_(const LinAlg::VectorBase&) const;
    double plogp_(size_t) const;

  }; // EntropyCalculator

} // namespace Stat

#endif // ENTROPY_H
