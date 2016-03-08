#if 0

#include "ggo_kernel_processing.h"
#include <ggo_memory.h>
#include <ggo_array.h>
#include <functional>

/*
//////////////////////////////////////////////////////////////
void ggo_build_luma_map(const uint8_t * r, const uint8_t * g, const uint8_t * b, int width, int height, ggo_array_uint16 & luma, ggo_user_feedback_abc * feedback)
{
  int	i, count;

  luma	= ggo_array_uint16( width*height );
  count	= width*height;
  for ( i = 0; i < count; ++i )
  {
    luma[i] = r[i] + g[i] + b[i];

    if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, count>>8 ) == false ) )
    {
      break;
    }
  }
}
*/

namespace
{
  //////////////////////////////////////////////////////////////
  template <typename T, typename Compare>
  void morpho_square_kernel_t(const T * input, T * output, int width, int height, int kernel_width, int kernel_height, Compare comp)
  {
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        output[y * width + x] = input[y * width + x];

        for (int kernel_y = -kernel_height; kernel_y <= kernel_height; ++kernel_y)
        {
          if ((y + kernel_y < 0) || (y + kernel_y >= height)) { continue; }

          for (int kernel_x = -kernel_width; kernel_x <= kernel_width; ++kernel_x)
          {
            if ((x + kernel_x < 0) || (x + kernel_x >= width)) { continue; }

            T cur = input[(y + kernel_y) * width + x + kernel_x];

            if (comp(cur, output[y * width + x]))
            {
              output[y * width + x] = cur;
            }
          }
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T, typename Compare>
  void morpho_disc_kernel_t(const T * input, T * output, int width, int height, float radius, Compare comp)
  {
    float radius_squared = radius * radius;
    int kernel_size = static_cast<int>(radius + 1);

    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        output[y * width + x] = input[y * width + x];

        for (int kernel_y = -kernel_size; kernel_y <= kernel_size; ++kernel_y)
        {
          for (int kernel_x = -kernel_size; kernel_x <= kernel_size; ++kernel_x)
          {
            float hypot = static_cast<float>(kernel_y * kernel_y + kernel_x * kernel_x);
            int input_x = x + kernel_x;
            int input_y = y + kernel_y;

            if (input_x >= 0 && input_x < width &&
                input_y >= 0 && input_y < height &&
                hypot <= radius_squared)
            {
              T cur = input[input_y * width + input_x];

              if (comp(cur, output[y * width + x]))
              {
                output[y * width + x] = cur;
              }
            }
          }
        }
      }
    }
  }
}


/*
//////////////////////////////////////////////////////////////
void ggo_build_jumble_noise_disps(int width, int height, ggo_array<ggo_vector2d_int> & disps, int kernel_width, int kernel_height, ggo_user_feedback_abc * feedback)
{
  int	x, y;

  disps = ggo_array<ggo_vector2d_int>( width*height );

  for ( y = 0; y < height; ++y )
  {
    if ( ( feedback != NULL ) && ( feedback->update( y, height ) == false ) )
    {
      break;
    }

    for ( x = 0; x < width; ++x )
    {
      disps[ ggo_get_2d_index( x, y, width ) ].x() = (rand()%(2*kernel_width +1))-kernel_width;
      disps[ ggo_get_2d_index( x, y, width ) ].y() = (rand()%(2*kernel_height+1))-kernel_height;
    }
  }
}*/

/*
//////////////////////////////////////////////////////////////
void ggo_build_median_cut_disps(const uint16_t * luma, int width, int height, ggo_array<ggo_vector2d_int> & disps, int kernel_width, int kernel_height, ggo_user_feedback_abc * feedback)
{
  int									x, y, kernel_x, kernel_y;
  vector<pair<int, ggo_vector2d_int> >	disps_luma;
  pair<int, ggo_vector2d_int>			cur_disp_luma;

  disps = ggo_array<ggo_vector2d_int>( width*height );

  for ( y = 0; y < height; ++y )
  {
    if ( ( feedback != NULL ) && ( feedback->update( y, height ) == false ) )
    {
      break;
    }

    for ( x = 0; x < width; ++x )
    {
      disps[ ggo_get_2d_index( x, y, width ) ].x() = 0;
      disps[ ggo_get_2d_index( x, y, width ) ].y() = 0;

      disps_luma.clear();

      for ( kernel_y = -kernel_height; kernel_y <= kernel_height; ++kernel_y )
      {
        if ( ( y+kernel_y < 0 ) || ( y+kernel_y >= height ) ) { continue; }

        for ( kernel_x = -kernel_width; kernel_x <= kernel_width; ++kernel_x )
        {
          if ( ( x+kernel_x < 0 ) || ( x+kernel_x >= width ) ) { continue; }

          cur_disp_luma.first			= luma[ ggo_get_2d_index( x+kernel_x, y+kernel_y, width ) ];
          cur_disp_luma.second.x()	= kernel_x;
          cur_disp_luma.second.y()	= kernel_y;

          disps_luma.push_back( cur_disp_luma );
        }
      }

      GGO_ASSERT( disps_luma.empty() == false );

      sort( disps_luma.begin(), disps_luma.end(), luma_disp_sort_func );

      disps[ ggo_get_2d_index( x, y, width ) ] = disps_luma[ disps_luma.size()/2 ].second;
    }
  }
}*/

namespace ggo
{
  //////////////////////////////////////////////////////////////
  void dilate_square_kernel(const uint8_t * input, uint8_t * output, int width, int height, int kernel_width, int kernel_height)
  {
    morpho_square_kernel_t(input, output, width, height, kernel_width, kernel_height, std::greater<int>());
  }

  //////////////////////////////////////////////////////////////
  void erode_square_kernel(const uint8_t * input, uint8_t * output, int width, int height, int kernel_width, int kernel_height)
  {
    morpho_square_kernel_t(input, output, width, height, kernel_width, kernel_height, std::less<int>());
  }

  //////////////////////////////////////////////////////////////
  void dilate_circle_kernel(const uint8_t * input, uint8_t * output, int width, int height, float radius)
  {
    morpho_disc_kernel_t(input, output, width, height, radius, std::greater<int>());
  }

  //////////////////////////////////////////////////////////////
  void erode_circle_kernel(const uint8_t * input, uint8_t * output, int width, int height, float radius)
  {
    morpho_disc_kernel_t(input, output, width, height, radius, std::less<int>());
  }
}


/*
//////////////////////////////////////////////////////////////
void	ggo_jumble_noise(uint8_t * r, uint8_t * g, uint8_t * b, int width, int height, int kernel_width, int kernel_height, ggo_user_feedback_abc * feedback)
{
  ggo_array<ggo_vector2d_int>	disps;

  ggo_build_jumble_noise_disps( width, height, disps, kernel_width, kernel_height, feedback );
  ggo_apply_disps( r, disps, width, height, feedback );
  ggo_apply_disps( g, disps, width, height, feedback );
  ggo_apply_disps( b, disps, width, height, feedback );
}

//////////////////////////////////////////////////////////////
void	ggo_median_cut(uint8_t * r, uint8_t * g, uint8_t * b, int width, int height, int kernel_width, int kernel_height, ggo_user_feedback_abc * feedback)
{
  ggo_array_uint16			luma;
  ggo_array<ggo_vector2d_int>	disps;

  ggo_build_luma_map( r, g, b, width, height, luma, feedback );
  ggo_build_median_cut_disps( luma, width, height, disps, kernel_width, kernel_height, feedback );
  ggo_apply_disps( r, disps, width, height, feedback );
  ggo_apply_disps( g, disps, width, height, feedback );
  ggo_apply_disps( b, disps, width, height, feedback );
}*/

#endif

