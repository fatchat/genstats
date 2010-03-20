#ifndef PTREE_H
#define PTREE_H

#include <cstdio>
#include <map>
#include "math/linalg.h"

namespace Stat {

  class PTree {

    class node {
      std::map<size_t, node*> children_;
      size_t count_;
      double prob_;

    public:
      node();
      ~node();

      size_t count() const { return count_; }
      double prob() const { return prob_; }
      node* child(size_t el);

      void inc() { ++count_; }
      void set_prob(size_t total_for_this_depth);
      void set_child_probs();
      node* add_child_count(size_t el);

      void show_probs(FILE*, size_t level) const;
    }; // node

    node *root_;

  public:
    PTree();
    ~PTree();
    
    void add_vector(const LinAlg::ConstVector&);
    void add_vectors(const LinAlg::System&);
    void calc_probs();

    void show_probs(FILE*) const;

  }; // PTree

} // Stat

#endif // PTREE_H
