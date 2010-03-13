#include <cassert>
#include <numeric>
#include <algorithm>
#include <functional>
#include <cmath>

#include "math/linalg.h"

namespace LinAlg {

  // ======================== System ========================
  std::size_t System::create()
  {
    const size_t sz = dim_ * next_index_;
    if(sz == store_.size()) {
      store_.resize(2 * sz + dim_);
    }
    return next_index_++;
  }

  Vector System::vec(size_t n)
  {
    assert(n < n_vectors());
    return Vector(n, *this);
  }

  System::const_iterator System::begin(size_t n) const
  {
    return &store_[n * dim_];
  }
  
  System::iterator System::begin(size_t n)
  {
    return &store_[n * dim_];
  }

  // ========================= Vector =========================
  Vector::Vector(size_t pos, System& las)
    : pos_(pos)
    , las_(las)
  { }
  
  Vector::Vector(System& las)
    : pos_(las.create())
    , las_(las)
  { }

  Vector::Vector(const Vector& v)
    : pos_(v.las_.create())
    , las_(v.las_)
  {
    std::copy(v.begin(), v.end(), this->begin());
  }

  System::const_element_ref Vector::operator[](size_t n) const
  {
    assert(n < las_.dim());
    return *(las_.begin(pos_) + n);
  }
  
  System::element_ref Vector::operator[](size_t n)
  {
    assert(n < las_.dim());
    return *(las_.begin(pos_) + n);
  }

  System::const_iterator Vector::begin() const
  {
    return las_.begin(pos_);
  }

  System::iterator Vector::begin()
  {
    return las_.begin(pos_);
  }

  System::const_iterator Vector::end() const
  {
    return las_.begin(pos_) + las_.dim();
  }

  System::iterator Vector::end()
  {
    return las_.begin(pos_) + las_.dim();
  }

  bool Vector::operator==(const Vector& rhs) const
  {
    if(&rhs != this) {
      for(size_t i = 0; i < las_.dim(); ++i) {
	if(this->operator[](i) != rhs[i]) {
	  return false;
	}
      }
    }
    return true;
  }

  // ======================== free functions ========================
  double innerprod(const Vector& lhs, const Vector& rhs)
  {
    return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), 0.0);
  }

  double norm(const Vector& v)
  {
    return sqrt(innerprod(v, v));
  }

  double norm(const System& s, size_t v_index)
  {
    double n = 0;
    for(size_t i = 0; i < s.dim(); ++i) {
      const double d = *(s.begin(v_index) + i);
      n += d*d;
    }
    return sqrt(n);
  }

  Vector operator*(double f, const Vector& v)
  {
    Vector w(v);
    std::transform(v.begin(), v.end(), w.begin(),
		   std::bind2nd(std::multiplies<double>(), f));
    return w;
  }

  void print_int_vector(const Vector& vec, FILE* fp)
  {
    for(System::const_iterator it = vec.begin(); it != vec.end(); ++it) {
      fprintf(fp, "%d ", (int)*it);
    }
    fprintf(fp, "\n");
  }

} // namespace
