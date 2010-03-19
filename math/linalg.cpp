#include <cassert>
#include <numeric>
#include <algorithm>
#include <functional>
#include <cmath>
#include <stdexcept>

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

  void System::index_check_(size_t n) const
  {
    if(n >= next_index_) {
      throw std::runtime_error("invalid index in LinAlg::System::begin()");
    }
  }

  System::const_iterator System::begin(size_t n) const
  {
    index_check_(n);
    return &store_[n * dim_];
  }
  
  System::iterator System::begin(size_t n)
  {
    index_check_(n);
    return &store_[n * dim_];
  }

  // ========================= Vector =========================
  Vector::Vector(size_t pos, System& las)
    : VectorBase(pos)
    , las_(las)
  {
    las_.begin(pos); // calls index_check_
  }
  
  Vector::Vector(System& las)
    : VectorBase(las.create())
    , las_(las)
  { }

  Vector::Vector(const Vector& v)
    : VectorBase(v.las_.create())
    , las_(v.las_)
  {
    std::copy(v.begin(), v.end(), this->begin());
  }

  void Vector::index_check_(size_t n) const
  {
    if(n >= las_.dim()) {
      throw std::runtime_error("invalid index in Stat::Vector::index_check_()");
    }
  }

  System::const_element_ref Vector::operator[](size_t n) const
  {
    index_check_(n);
    return *(las_.begin(pos_) + n);
  }
  
  System::element_ref Vector::operator[](size_t n)
  {
    index_check_(n);
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

  bool Vector::operator==(const VectorBase& rhs) const
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

  // ========================= ConstVector =========================
  ConstVector::ConstVector(size_t pos, const System& las)
    : VectorBase(pos)
    , las_(las)
  {
    las_.begin(pos); // calls index_check_
  }

  ConstVector::ConstVector(const ConstVector& vec)
    : VectorBase(vec.pos_)
    , las_(vec.las_)
  { }

  void ConstVector::index_check_(size_t n) const
  {
    if(n >= las_.dim()) {
      throw std::runtime_error("invalid index in Stat::ConstVector::index_check_()");
    }
  }

  System::const_element_ref ConstVector::operator[](size_t n) const
  {
    index_check_(n);
    return *(las_.begin(pos_) + n);
  }

  System::const_iterator ConstVector::begin() const
  {
    return las_.begin(pos_);
  }

  System::const_iterator ConstVector::end() const
  {
    return las_.begin(pos_) + las_.dim();
  }

  bool ConstVector::operator==(const VectorBase& rhs) const
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

  void print_int_vector(const VectorBase& vec, FILE* fp)
  {
    for(System::const_iterator it = vec.begin(); it != vec.end(); ++it) {
      fprintf(fp, "%d ", (int)*it);
    }
    fprintf(fp, "\n");
  }

} // namespace
