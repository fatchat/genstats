#include <cmath>
#include "math/entropy.h"

#if 0
#define DEBUG_VP
#endif

#ifdef DEBUG_VP
#include <cstdio>
#endif

namespace Stat {

  EntropyCalculator::EntropyCalculator(size_t nletters,
				       LinAlg::System::ptr vectors)
    : vectors_(vectors)
    , buckets_(vectors->dim(), nletters)
  {
    for(size_t i = 0; i < vectors->n_vectors(); ++i) {
      const LinAlg::ConstVector vec(i, *vectors);
      buckets_.count(vec);
    }
    buckets_.verify_sums(true);
  }

  double EntropyCalculator::vector_prob_(const LinAlg::VectorBase& vec) const
  {
    double prob = 1.0;
#ifdef DEBUG_VP
    printf("prob [");
#endif
    for(size_t position = 0; position < vec.dim(); ++position) {
      const size_t symbol = vec[position];
#ifdef DEBUG_VP
      printf("%d ", (int)symbol);
#endif
      prob *= buckets_.prob(symbol, position);
    }
#ifdef DEBUG_VP
    printf("] = %f\n", prob);
#endif
    return prob;
  }

  double EntropyCalculator::plogp_(size_t i) const
  {
    const LinAlg::ConstVector vec(i, *vectors_);
    const double p = vector_prob_(vec);
    return p ? p * log(p) * -1.0 : 0.0;
  }

  double EntropyCalculator::entropy() const
  {
    double entropy = 0.0;
    for(size_t i = 0; i < vectors_->n_vectors(); ++i) {
      entropy += plogp_(i);
    }
    return entropy;
  }

  double EntropyCalculator::prob_check() const
  {
    double prob = 0.0;
    for(size_t i = 0; i < vectors_->n_vectors(); ++i) {
      const LinAlg::ConstVector vec(i, *vectors_);
      const double p = vector_prob_(vec);
      prob += p;
    }
    return prob;
  }

} // namespace
