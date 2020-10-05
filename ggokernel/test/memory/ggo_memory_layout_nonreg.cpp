#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/memory/ggo_ptr_arithmetics.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/nonreg/ggo_nonreg.h>

namespace ggo
{
  template <int item_byte_size_>
  struct bottom_up_memory_layout
  {
    static constexpr int item_byte_size = item_byte_size_;
    
    bottom_up_memory_layout(int line_byte_step) : _line_byte_step(line_byte_step) {}
    
    template <typename void_ptr_t, typename func>
    void for_each(void_ptr_t ptr, rect_int rect, func && f) const
    {
      static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);
      
      const int b = rect.bottom();
      const int t = rect.top();
      const int l = rect.left();
      const int r = rect.right();
      
      for (int y = b; y <= t; ++y)
      {
        void_ptr_t it = move_ptr(ptr, y * _line_byte_step + l * item_byte_size);
        
        for (int x = l; x <= r; ++x)
        {
          f(it, x, y);
          it = move_ptr<item_byte_size>(it);
        }
      }
    }
    
    template <typename void_ptr_t>
    auto at(void_ptr_t * ptr, int x, int y) const
    {
      static_assert(std::is_same_v<void_ptr_t, void *> || std::is_same_v<void_ptr_t, const void *>);
      
      return move_ptr(ptr, y * _line_byte_step + x * item_byte_size);
    }
    
    int buffer_byte_size(size s) const
    {
      return _line_byte_step * s.height();
    }

    const int _line_byte_step;
  };
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(memory_layouts, bottom_up_buffer_byte_size)
{
  ggo::bottom_up_memory_layout<2> memory_layout(10);
  
  GGO_CHECK_EQ(memory_layout.buffer_byte_size({4, 3}), 30);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(memory_layouts, for_each)
{
  ggo::bottom_up_memory_layout<2> memory_layout(10);
  
  ggo::array_8u a(30);
  a.fill(0);
  void * buffer = a.data();
  
  std::vector<ggo::vec2_i> pixels;
  
  memory_layout.for_each(buffer, ggo::rect_int::from_left_right_bottom_top(0, 2, 1, 2), [&](void * ptr, int x, int y)
  {
    pixels.push_back(ggo::vec2_i(x, y));
    uint8_t * ptr_u8 = static_cast<uint8_t *>(ptr);
    *ptr_u8 = 1;
    
  });
  
  const std::vector<ggo::vec2_i> expected{
    {0, 1}, {1, 1}, {2,1},
    {0, 2}, {1, 2}, {2,2} };
  
  GGO_CHECK_EQ(pixels, expected);
}

