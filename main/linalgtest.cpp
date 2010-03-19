#include <cstdio>
#include <cmath>

#include "math/linalg.h"

int main()
{
  const size_t dim = 20;
  LinAlg::System las(dim);
  assert(las.dim() == dim);
  assert(las.n_vectors() == 0);

  const size_t first = las.create();
  assert(first == 0);
  assert(las.n_vectors() == 1);

  LinAlg::Vector v(first, las);
  // check Vector::operator[]
  for(size_t i = 0; i < dim; ++i) {
    v[i] = 0.1 * i;
  }

  // check Vector::operator[] const
  for(size_t i = 0; i < dim; ++i) {
    assert(v[i] == 0.1 * i);
  }

  // check Vector::begin() and end()
  std::vector<double> dest(dim);
  std::copy(v.begin(), v.end(), dest.begin());
  for(size_t i = 0; i < dim; ++i) {
    assert(dest[i] == 0.1 * i);
  }

  // check Vector::operator==
  LinAlg::Vector t(las.create(), las);
  assert(las.n_vectors() == 2);
  for(size_t i = 0; i < dim; ++i) {
    t[i] = 0.1 * i;
  }
  assert(v == t);

  // check Vector(const Vector&)
  LinAlg::Vector u(v);
  assert(las.n_vectors() == 3);
  assert(u == v);
  assert(v == u);

  // check operator*
  LinAlg::Vector vv(-2.32 * v);
  assert(las.n_vectors() == 4);
  for(size_t i = 0; i < dim; ++i) {
    assert(vv[i] == -2.32 * v[i]);
  }

  // check innerprod
  LinAlg::Vector ww(las);
  assert(las.n_vectors() == 5);
  for(size_t i = 0; i < dim; ++i) {
    ww[i] = i + 1;
  }
  assert(innerprod(ww, ww) == dim * (dim + 1) * (2.0 * dim + 1) / 6);

  // check innerprod
  LinAlg::Vector w1(las);
  LinAlg::Vector w2(las);
  assert(las.n_vectors() == 7);
  for(size_t i = 0; i < dim; ++i) {
    w1[i] = (2 * i + 1) * (2 * i + 1);
    w2[i] = pow(-1.0, i);
  }
  if(dim %2) {    assert(innerprod(w1, w2) == 2 * dim * dim - 1);  }
  else {    assert(innerprod(w1, w2) == -2 * double(dim * dim));  }

  // check norm
  assert(LinAlg::norm(v) == LinAlg::norm(las, first));

  // check norm
  LinAlg::Vector w(las);
  assert(las.n_vectors() == 8);
  assert(LinAlg::norm(w) == 0.0);
  
}
