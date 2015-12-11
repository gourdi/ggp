#ifndef __GGO_TREE__
#define __GGO_TREE__

#include <ggo_memory.h>
#include <ggo_kernel.h>

namespace ggo
{
  template<typename T>
  class tree
  {
  public:
  
                                    tree(const T & data);
                                   ~tree();
            
    bool								            is_leaf() const;
    
    T &									            data() { return _data; }
    const T &							          data() const { return _data; }
    
    std::vector<tree<T> *> &		    subtrees() { return _subtrees; }
    const std::vector<tree<T> *> &  subtrees() const { return _subtrees; }
    
    tree<T> *						            create_leaf(const T & data);
    
    void								            get_leaves_data(std::vector<T> & data) const;
      
    template <typename Func>
    void                            visit_leaves(Func f)
    {
      if (is_leaf() == true)
      {
        f(this);
      }
      else
      {
        for (tree<T> * subtree : _subtrees)
        {
          subtree->visit_leaves(f);
        }
      }
    }
      
  private:
  
    T 							        _data;
    tree<T> *               _parent;
    std::vector<tree<T> *>	_subtrees;
  };
  
  /////////////////////////////////////////////////////////////////////
  template <class T>
  tree<T>::tree(const T & data)
  :
  _data(data),
  _parent(nullptr)
  {
  }
  
  /////////////////////////////////////////////////////////////////////
  template <class T>
  tree<T>::~tree()
  {
    for (tree<T> * subtree : _subtrees)
    {
      delete subtree;
    }
  }
  
  /////////////////////////////////////////////////////////////////////
  template <class T>
  bool tree<T>::is_leaf() const
  {
    return _subtrees.size() == 0;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <class T>
  tree<T> * tree<T>::create_leaf(const T & data)
  {
    tree<T> * leaf = new tree<T>(data);
    leaf->_parent = this;
    _subtrees.push_back(leaf);
    return leaf;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <class T>
  void tree<T>::get_leaves_data(std::vector<T> & data) const
  {
    if (is_leaf() == true)
    {
      data.push_back(_data);
    }
    else
    {
      for (const tree<T> * subtree : _subtrees)
      {
        subtree->get_leaves_data(data);
      }
    }
  }
}

#endif

