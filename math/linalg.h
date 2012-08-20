#ifndef LINALG_H
#define LINALG_H

#include <cstdio>
#include <vector>
#include <boost/smart_ptr.hpp>

namespace LinAlg {

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
    
    // create new vector and return new index
    size_t create();

    // create difference and return new index
    size_t difference(size_t index_1, size_t index_2);

    const_iterator begin(size_t n) const;
    iterator begin(size_t n);

  private:
    void index_check_(size_t) const;
    // no impl
    System();
    System(const System&);
    System& operator=(const System&);

  }; // System

  // ========================== VectorBase =============================
  class VectorBase {
  protected:
    const size_t pos_;
  public:
    VectorBase(size_t pos) : pos_(pos) { }
    virtual ~VectorBase() { }
    virtual size_t dim() const = 0;
    virtual void index_check_(size_t) const = 0;
    virtual System::const_element_ref operator[](size_t n) const = 0;
    virtual System::const_iterator begin() const = 0;
    virtual System::const_iterator end() const = 0;
    virtual bool operator==(const VectorBase&) const = 0;
  };

  // ============================ Vector  ==============================
 class Vector : public VectorBase {
    System& las_;

    // no impl
    Vector();
    Vector& operator=(const Vector&);
  public:
    Vector(size_t pos, System&);
    Vector(System&);
    Vector(const Vector&);
    size_t dim() const { return las_.dim(); }
    System::const_element_ref operator[](size_t n) const;
    System::element_ref operator[](size_t n);

    System::const_iterator begin() const;
    System::iterator begin();

    System::const_iterator end() const;
    System::iterator end();

    bool operator==(const VectorBase&) const;

  private:
    virtual void index_check_(size_t) const;
  }; // Vector

  // ============================ ConstVector  ==============================
 class ConstVector : public VectorBase {
    const System& las_;

    // no impl
    ConstVector();
    ConstVector& operator=(const ConstVector&);
  public:
    ConstVector(size_t pos, const System&);
    ConstVector(const ConstVector&);
    size_t dim() const { return las_.dim(); }

    System::const_element_ref operator[](size_t n) const;
    System::const_iterator begin() const;
    System::const_iterator end() const;

    bool operator==(const VectorBase&) const;

  private:
    virtual void index_check_(size_t) const;
  }; // ConstVector

  // ========================= free functions ============================
  double norm(const Vector&);
  double norm(const System&, size_t vector_index);
  Vector operator*(double, const Vector&);
  double innerprod(const Vector&, const Vector&);

  void print_int_vector(const VectorBase& vec, FILE* fp);
  size_t count_letters(const System&);

} // namespace LinAlg

#endif // LINALG_H
