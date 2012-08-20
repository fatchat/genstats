#ifndef PTREE_H
#define PTREE_H

#include <cstdio>
#include <vector>
#include <map>
#include "math/linalg.h"

namespace Stat {

  class PTree {

    class storage;
    // ============================= node ===============================
    class node {
    public:
      typedef int el_type;
      typedef size_t ind_type;
      typedef std::map<el_type, ind_type> nodemap;

    private:
      nodemap children_;
      size_t count_;
      double prob_;

    public:
      // required for std::vector<node>
      node();
      node(const node&);
      node& operator=(const node&);

      bool operator==(const node&) const;

      size_t   count         ()           const { return count_; }
      double   prob          ()           const { return prob_; }
      ind_type get_child_ind (el_type el) const;

      void     inc              () { ++count_; }
      void     set_prob         (size_t total_for_this_depth);
      void     set_child_probs  (storage&);
      void     create_new_child (el_type, ind_type);

      void     show         (FILE*, size_t) const;
      void     show_probs   (FILE*, size_t level, const storage&) const;

    }; // node

    // ============================= storage ===============================
    class storage {

      std::vector<node> nodestore_;
      size_t next_;

    public:
      storage()
		: next_(0)
		{ }

      size_t        next_index     ();

      node&         get_node_ref   (size_t ind);
      const node&   get_node       (size_t ind) const;

      void          show           (FILE*) const;

    }; // storage

    // ============================= PTree ================================
    storage                storage_;
    node::ind_type         root_;

  public:
    PTree();

    void add_vector          (const LinAlg::ConstVector&);
    void add_vectors         (const LinAlg::System&);
    void calc_probs          ();
    double vector_prob       (const LinAlg::VectorBase&) const;

    void show_probs          (FILE*) const;
    void show_storage        (FILE*) const;
  }; // PTree

} // Stat

#endif // PTREE_H
