#ifndef LINALG_H
#define LINALG_H

#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/numeric/ublas/vector.hpp>

template<class T>
class LinAlgSys {
 public:
  const std::size_t dim;

 public:
  typedef boost::numeric::ublas::vector<T>  vector_type;

 public:
  LinAlgSys(std::size_t _dim)
    : dim(_dim) { }

  vector_type make() const
  { return vector_type(dim); }
  vector_type zero() const
  { return boost::numeric::ublas::zero_vector<T>(dim); }
  vector_type scalar(T val) const
  { return boost::numeric::ublas::scalar_vector<T>(dim, val); }

 public:
  typedef boost::shared_ptr<vector_type>   ptr_vector_type;

 private:
  std::vector< ptr_vector_type >           vectors_;

 public:
  size_t n_vectors() const
  { return vectors_.size(); }

  std::size_t create_and_store_vector()
  { vectors_.push_back(ptr_vector_type(new vector_type(dim)));
    return vectors_.size() - 1; }

  vector_type& access_vector_ref(std::size_t index)
    { return *(vectors_.at(index)); }
  
  const vector_type& access_vector(std::size_t index) const
  { return *(vectors_.at(index)); }
  
  T norm(std::size_t index) const
  { return boost::numeric::ublas::norm_2(access_vector(index)); }

}; // LinAlgSys

#endif // LINALG_H
