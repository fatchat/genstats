#include <stdexcept>

#include "math/buckets.h"

namespace Stat {

  Buckets::Buckets(size_t dim)
    : buckets_(dim)
    , created_(false)
  { }

  void Buckets::create(size_t n)
  {
    if(created_) {
      throw std::runtime_error("called Stat::Buckets::create() twice");
    }
    while(n--) {
      buckets_.create();
    }
    created_ = true;
  }

  void Buckets::count(const LinAlg::Vector& v)
  {
    size_t d = buckets_.dim();
    do {
      --d;
      ++*(buckets_.begin((size_t)v[d]) + d);
    } while (d > 0);
  }

  void Buckets::print(size_t n, FILE* fp)
  {
    if(!fp) {
      throw std::runtime_error("null FILE* passed to Stat::Buckets::print");
    }
    if(n < 0 || n >= buckets_.n_vectors()) {
      throw std::runtime_error("index out of range in Stat::Buckets::print");
    }
    LinAlg::Vector v(n, buckets_);
    LinAlg::print_int_vector(v, fp);
  }

  bool Buckets::verify_sums() const
  {
    const size_t n = buckets_.n_vectors();
    const size_t d = buckets_.dim();
    size_t actual = 0;
    for(size_t i = 0; i < d; ++i) {
      size_t sum = 0;
      for(size_t j = 0; j < n; ++j) {
	sum += *(buckets_.begin(j) + i);
      }
      if(i == 0) {
	actual = sum;
      }
      else if(sum != actual) {
	return false;
      }
    }
    return true;
  }

} // namespace