#include <string>
#include <stdexcept>

#include "math/ptree.h"

namespace Stat {

  // ============================ PTree::node ==============================
  PTree::node::node()
    : count_(0)
    , prob_(0.0)
  { }

  PTree::node::node(const node& n)
    : children_(n.children_)
    , count_(n.count_)
    , prob_(n.prob_)
  { }

  PTree::node& PTree::node::operator=(const PTree::node& rhs)
  {
    if(&rhs != this) {
      children_ = rhs.children_;
      count_ = rhs.count_;
      prob_ = rhs.prob_;
    }
    return *this;
  }

  bool PTree::node::operator==(const PTree::node& rhs) const
  {
    if(&rhs == this) {
      return true;
    }
    return (count_ == rhs.count_ &&
	    prob_ == rhs.prob_ &&
	    children_ == rhs.children_);
  }

  PTree::node::ind_type PTree::node::get_child_ind(PTree::node::el_type el) const
  {
    const nodemap::const_iterator it = children_.find(el);
    if(it == children_.end()) {
      return 0;
    }
    assert(it->second); // root_ is never a child
    return it->second;
  }

  void PTree::node::create_new_child(PTree::node::el_type el, PTree::node::ind_type next)
  {
    assert(next);
    assert(children_.find(el) == children_.end());
    children_[el] = next;
  }

  void PTree::node::set_prob(size_t total)
  {
    assert(count_ > 0); // never called on root node
    assert(total >= count_);
    prob_ = count_ * 1.0 / total;
  }

  void PTree::node::set_child_probs(PTree::storage& store)
  {
    size_t total = 0;

    for(nodemap::const_iterator it = children_.begin(); it != children_.end(); ++it)
      {
	total += store.get_node(it->second).count();
      }

    for(nodemap::iterator it = children_.begin(); it != children_.end(); ++it)
      {
	assert(it->second);
	store.get_node_ref(it->second).set_prob(total);
	store.get_node_ref(it->second).set_child_probs(store);
      }
  }

  void PTree::node::show(FILE* fp, size_t ind) const
  {
    if(fp) {
      fprintf(fp, "node %d: count_=%d, prob_=%1.2f\n",
	      (int)ind, (int)count_, prob_);
      typedef nodemap::const_iterator iterator;
      for(iterator it = children_.begin(); it != children_.end(); ++it) {
	printf("node %d has child %d holding %d\n",
	       (int)ind, (int)it->second, (int)it->first);
      }
      printf("end of node %d\n", (int)ind);
    }
  }

  void PTree::node::show_probs(FILE* fp, size_t level, const PTree::storage& store) const
  {
    if(!fp) return;
    const std::string spacing(2 * level, ' ');
    for(nodemap::const_iterator it = children_.begin(); it != children_.end(); ++it)
      {
	const node& n = store.get_node(it->second);
	fprintf(fp, "%sP[V_%d = %d | branch] = %1.3f\n",
		spacing.c_str(),
		(int)level, (int)it->first, n.prob());
	n.show_probs(fp, 1 + level, store);
      }
  }

  // =========================== PTree::storage ==========================
  size_t PTree::storage::next_index()
  {
    assert(next_ <= nodestore_.size());
    if(next_ == nodestore_.size()) {
      nodestore_.resize(next_ * 2 + 1);
    }
    assert(next_ < nodestore_.size());
    return next_++;
  }

  PTree::node& PTree::storage::get_node_ref(size_t ind)
  {
    if(ind >= next_) {
      throw std::runtime_error("bad index in PTree::storage::get_node_ref");
    }
    return nodestore_.at(ind);
  }

  const PTree::node& PTree::storage::get_node(size_t ind) const
  {
    if(ind >= next_) {
      throw std::runtime_error("bad index in PTree::storage::get_node");
    }
    return nodestore_.at(ind);
  }

  void PTree::storage::show(FILE* fp) const
  {
    if(fp) {
      for(size_t i = 0; i < next_; ++i) {
	nodestore_.at(i).show(fp, i);
      }
    }
  }

  // ============================== PTree ================================
  PTree::PTree()
  {
    root_ = storage_.next_index();
    assert(root_ == 0);
  }

  void PTree::add_vector(const LinAlg::ConstVector& cv)
  {
    //printf("adding vector:");  LinAlg::print_int_vector(cv, stdout);
    node::ind_type current_ind = root_;

    for(size_t i = 0; i < cv.dim(); ++i) {
      node::ind_type child_ind = storage_.get_node(current_ind).get_child_ind(cv[i]);
      if(child_ind == 0) {
	const node::ind_type next_ind = storage_.next_index();
	storage_.get_node_ref(current_ind).create_new_child(cv[i], next_ind);
	child_ind = storage_.get_node(current_ind).get_child_ind(cv[i]);
	assert(child_ind == next_ind);
      }
      assert(child_ind);
      storage_.get_node_ref(child_ind).inc();
      current_ind = child_ind;
    }
  }

  double PTree::vector_prob(const LinAlg::VectorBase& cv) const
  {
    double prob = 1.0;
    node::ind_type current_ind = root_;
    for(size_t i = 0; i < cv.dim(); ++i) {
      node::ind_type child_ind = storage_.get_node(current_ind).get_child_ind(cv[i]);
      if(child_ind == 0) {
	return 0.0;
      }
      prob *= storage_.get_node(child_ind).prob();
      current_ind = child_ind;
    }
    return prob;
  }

  void PTree::add_vectors(const LinAlg::System& las)
  {
    for(size_t i = 0; i < las.n_vectors(); ++i) {
      const LinAlg::ConstVector cv(i, las);
      add_vector(cv);
    }
  }

  void PTree::calc_probs()
  {
    storage_.get_node_ref(root_).set_child_probs(storage_);
  }

  void PTree::show_probs(FILE* fp) const
  {
    storage_.get_node(root_).show_probs(fp, 0, storage_);
  }

  void PTree::show_storage(FILE* fp) const
  {
    storage_.show(fp);
  }

} // Stat
