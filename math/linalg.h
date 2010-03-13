#ifndef LINALG_H
#define LINALG_H

#include <vector>
#include <boost/smart_ptr.hpp>

namespace LinAlg {

  class Vector;

  // ============================ System  ==============================
  class System {
    
    typedef double T;

  public:
    typedef T*       iterator;
    typedef const T* const_iterator;
    typedef T&       element_ref;
    typedef const T& const_element_ref;
    typedef boost::shared_ptr<System> ptr;

  private:
    const std::size_t dim_;
    size_t next_index_;
    std::vector<T> store_;
    
  public:
    System(std::size_t dim)
      : dim_(dim)
      , next_index_(0)
      { }
    
    size_t dim() const { return dim_; }
    size_t n_vectors() const { return next_index_; }
    
    size_t create();
    Vector vec(size_t);

    const_iterator begin(size_t n) const;
    iterator begin(size_t n);

  private:
    // no impl
    System();
    System(const System&);
    System& operator=(const System&);

  }; // System

  // ============================ Vector  ==============================
  class Vector {
    size_t pos_;
    System& las_;

    // no impl
    Vector();
    Vector& operator=(const Vector&);
  public:
    Vector(size_t pos, System&);
    Vector(System&);
    Vector(const Vector&);

    System::const_element_ref operator[](size_t n) const;
    System::element_ref operator[](size_t n);

    System::const_iterator begin() const;
    System::iterator begin();

    System::const_iterator end() const;
    System::iterator end();

    bool operator==(const Vector&) const;
  }; // Vector

  // ========================= free functions ============================
  double norm(const Vector&);
  double norm(const System&, size_t vector_index);
  Vector operator*(double, const Vector&);
  double innerprod(const Vector&, const Vector&);

} // namespace LinAlg

#endif // LINALG_H
