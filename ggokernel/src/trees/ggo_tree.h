#ifndef __GGO_TREE__
#define __GGO_TREE__

#include <ggo_kernel.h>

namespace ggo
{
  template<typename data_t>
  class tree final
  {
  public:
  
                                      tree(const data_t & data);
                                     
    bool								              is_leaf() const;
                                     
    data_t &									        data() { return _data; }
    const data_t &							      data() const { return _data; }
    
    std::vector<tree<data_t>> &		    subtrees() { return _subtrees; }
    const std::vector<tree<data_t>> & subtrees() const { return _subtrees; }
    
    tree<data_t> &						        create_leaf(const data_t & data);
    
    void								              get_leaves_data(std::vector<data_t> & data) const;
      
    template <typename func_t>
    void                              visit_leaves(func_t f);

    template <typename func_t>
    void                              visit_leaves(func_t f) const;
      
  private:
  
    data_t 							      _data;
    std::vector<tree<data_t>> _subtrees;
  };
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  tree<data_t>::tree(const data_t & data)
  :
  _data(data)
  {
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool tree<data_t>::is_leaf() const
  {
    return _subtrees.size() == 0;
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  tree<data_t> & tree<data_t>::create_leaf(const data_t & data)
  {
    _subtrees.emplace_back(data);
    return _subtrees.back();
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  void tree<data_t>::get_leaves_data(std::vector<data_t> & data) const
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
  template <typename data_t>
  template <typename func_t>
  void tree<data_t>::visit_leaves(func_t f)
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

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  template <typename func_t>
  void tree<data_t>::visit_leaves(func_t f) const
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

