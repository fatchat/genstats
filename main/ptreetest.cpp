#include <cstdio>

#include "math/linalg.h"
#include "math/ptree.h"

int main()
{
  const size_t dim = 2;
  LinAlg::System las(dim);

  LinAlg::Vector v1(las);
  v1[0] = 1;
  v1[1] = 0;
  LinAlg::Vector v2(las);
  v2[0] = 1;
  v2[1] = 1;

  Stat::PTree ptree;
  printf("adding vectors to ptree\n");
  ptree.add_vectors(las);
  //  printf("show_storage\n");
  //  ptree.show_storage(stdout);
  printf("calculating probs\n");
  ptree.calc_probs();
  printf("showing tree\n");
  ptree.show_probs(stdout);
  printf("prob of v1 = %1.2f\n", ptree.vector_prob(v1));
  printf("prob of v2 = %1.2f\n", ptree.vector_prob(v2));
  LinAlg::Vector v3(las);
  v3[0] = 2; v3[1] = 2;
  printf("prob of v3 = %1.2f\n", ptree.vector_prob(v3));
  return 0;
}
