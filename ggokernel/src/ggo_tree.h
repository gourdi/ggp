#ifndef __GGO_TREE__
#define __GGO_TREE__

#include <ggo_memory.h>
#include <ggo_kernel.h>

namespace ggo
{
  template<typename T>
  class tree final
  {
  public:
  
                                  tree(const T & data);
            
    bool								          is_leaf() const;
    
    T &									          data() { return _data; }
    const T &							        data() const { return _data; }
    
    std::vector<tree<T>> &		    subtrees() { return _subtrees; }
    const std::vector<tree<T>> &  subtrees() const { return _subtrees; }
    
    tree<T> &						          create_leaf(const T & data);
    
    void								          get_leaves_data(std::vector<T> & data) const;
      
    template <typename Func>
    void                          visit_leaves(Func f);
      
  private:
  
    T 							      _data;
    std::vector<tree<T>>  _subtrees;
  };
  
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  tree<T>::tree(const T & data)
  :
  _data(data)
  {
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  bool tree<T>::is_leaf() const
  {
    return _subtrees.size() == 0;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  tree<T> & tree<T>::create_leaf(const T & data)
  {
    _subtrees.emplace_back(data);
    return _subtrees.back();
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void tree<T>::get_leaves_data(std::vector<T> & data) const
  {
    if (is_leaf() == true)
    {
      data.push_back(_data);
    }
    else
    {
      for (const auto & subtree : _subtrees)
      {
        subtree.get_leaves_data(data);
      }
    }
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  template <typename Func>
  void tree<T>::visit_leaves(Func f)
  {
    if (is_leaf() == true)
    {
      f(*this);
    }
    else
    {
      for (auto & subtree : _subtrees)
      {
        subtree.visit_leaves(f);
      }
    }
  }
}

#endif

