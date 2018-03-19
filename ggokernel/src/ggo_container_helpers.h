#ifndef __GGO_CONTAINER_HELPERS__
#define __GGO_CONTAINER_HELPERS__

namespace ggo
{
  template <typename container_t, typename data_t>
  void remove(container_t & container, const data_t & v)
  {
    auto new_end = std::remove(container.begin(), container.end(), v);

    container.erase(new_end, container.end()); // Because std::remove_if does not remove, it just moves actually.
  }

  template <typename container_t, typename predicate_t>
  void remove_if(container_t & container, predicate_t predicate)
  {
    auto new_end = std::remove_if(container.begin(), container.end(), predicate);

    container.erase(new_end, container.end()); // Because std::remove_if does not remove, it just moves actually.
  }

  template <typename container_t, typename predicate_t>
  void remove_first_if(container_t & container, predicate_t predicate)
  {
    for (auto it = container.begin(); it != container.end(); ++it)
    {
      if (predicate(*it) == true)
      {
        container.erase(it);
        return;
      }
    }
  }

  template <typename container_t>
  void shuffle(container_t & container)
  {
    std::shuffle(container.begin(), container.end(), get_random_generator());
  }

  template <typename container_t, typename predicate_t>
  void sort(container_t & container, predicate_t predicate)
  {
    std::sort(container.begin(), container.end(), predicate);
  }

  template <typename container_t, typename predicate_t>
  bool find_if(const container_t & container, predicate_t predicate)
  {
    return std::find_if(container.begin(), container.end(), predicate) != container.end();
  }

  template <typename container_t, typename data_t>
  bool find(const container_t & container, const data_t & v)
  {
    return std::find(container.begin(), container.end(), v) != container.end();
  }

  template <typename container_t, typename data_t>
  void push_once(container_t & container, const data_t & v)
  {
    if (std::find(container.begin(), container.end(), v) == container.end())
    {
      container.push_back(v);
    }
  }

  template <typename container_t, typename data_t>
  void fill(container_t & container, const data_t & v)
  {
    std::fill(container.begin(), container.end(), v);
  }

  template <typename container_t, typename func_t, typename data_t>
  data_t accumulate(container_t & container, func_t f, data_t init)
  {
    return std::accumulate(container.begin(), container.end(), init, f);
  }

  template <typename container_t, typename predicate_t>
  void remove_duplicates(container_t & container, predicate_t predicate)
  {
    auto remove_duplicate = [&]()
    {
      for (auto it = container.begin(); it != container.end(); ++it)
      {
        auto search_predicate = [&](const auto & v) { return predicate(v, *it); };
        auto duplicate = std::find_if(it + 1, container.end(), search_predicate);
        if (duplicate != container.end())
        {
          container.erase(duplicate);
          return true;
        }
      }
      return false;
    };

    while (remove_duplicate() == true) {}
  }

  template <typename container_t>
  void remove_duplicates(container_t & container)
  {
    remove_duplicates(container, [](const auto & a, const auto & b) { return a == b; });
  }
}

// Adaptor.
namespace ggo
{
  template <typename container_t, typename func_t>
  class adaptor
  {
  public:

    using value_type = typename container_t::value_type;

    class iterator
    {
    public:

      iterator(typename container_t::const_iterator it, const func_t & func) : _it(it), _func(func) {}

      auto operator*() const { return _func(*_it); }

      void operator--() { --_it; }
      void operator++() { ++_it; }
      iterator operator++(int) { auto tmp = *this; ++_it; return tmp; }
      iterator operator--(int) { auto tmp = *this; --_it; return tmp; }
      bool operator==(const iterator & it) { return _it == it._it; }
      bool operator!=(const iterator & it) { return _it != it._it; }

    private:

      typename container_t::const_iterator _it;
      const func_t & _func;
    };

    adaptor(const container_t & container, const func_t & func) : _container(container), _func(func) {}

    auto begin() const { return iterator(_container.begin(), _func); }
    auto end() const { return iterator(_container.end(), _func); }

    bool empty() const { return _container.empty(); }

  private:

    const container_t & _container;
    const func_t & _func;
  };

  template <typename container_t, typename func_t>
  auto make_adaptor(const container_t & container, func_t func)
  {
    return adaptor<container_t, func_t>(container, func);
  }

  template <typename container_t>
  auto make_pointer_adaptor(const container_t & container)
  {
    return make_adaptor(container, [](const auto & item) { return &item; });
  }
}

#endif
