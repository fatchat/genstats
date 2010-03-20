#include <string>
#include "math/ptree.h"

namespace Stat {

  // ============================ PTree::node ==============================
  PTree::node::node()
    : count_(0)
    , prob_(0.0)
  { }

  PTree::node::~node()
  {
    for(std::map<size_t, node*>::iterator it = children_.begin();
	it != children_.end(); ++it)
      {
	if(it->second) {
	  delete it->second;
	  it->second = 0;
	}
      }
  }

  PTree::node* PTree::node::child(size_t el)
  {
    if(!children_[el]) {
      children_[el] = new node();
    }
    return children_[el];
  }

  void PTree::node::set_prob(size_t total)
  // never called on root node
  {
    assert(count_ > 0);
    assert(total >= count_);
    prob_ = count_ * 1.0 / total;
  }

  void PTree::node::set_child_probs()
  {
    size_t total = 0;

    typedef std::map<size_t, node*>::iterator iterator;
    const iterator end = children_.end();

    for(iterator it = children_.begin(); it != end; ++it)
      {
	assert(it->second);
	total += it->second->count();
      }

    for(iterator it = children_.begin(); it != end; ++it)
      {
	assert(it->second);
	it->second->set_prob(total);
	it->second->set_child_probs();
      }
  }

  PTree::node* PTree::node::add_child_count(size_t el)
  {
    node* c = child(el);
    assert(c);
    c->inc();
    return c;
  }

  void PTree::node::show_probs(FILE* fp, size_t level) const
  {
    if(!fp) return;
    typedef std::map<size_t, node*>::const_iterator iterator;
    const iterator end = children_.end();
    const std::string spacing(2 * level, ' ');
    for(iterator it = children_.begin(); it != end; ++it)
      {
	assert(it->second);
	fprintf(fp, "%sP[V_%d = %d | branch] = %1.3f\n",
		spacing.c_str(),
		(int)level, (int)it->first, it->second->prob());
	it->second->show_probs(fp, 1 + level);
      }
  }

  // ============================== PTree ================================
  PTree::PTree()
    : root_(new node())
  { }

  PTree::~PTree()
  {
    if(root_) {
      delete root_;
      root_ = 0;
    }
  }
  
  void PTree::add_vector(const LinAlg::ConstVector& cv)
  {
    node* current = root_;
    for(size_t i = 0; i < cv.dim(); ++i) {
      assert(current);
      current = current->add_child_count(cv[i]);
    }
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
    assert(root_);
    root_->set_child_probs();
  }

  void PTree::show_probs(FILE* fp) const
  {
    assert(root_);
    root_->show_probs(fp, 0);
  }

} // Stat
