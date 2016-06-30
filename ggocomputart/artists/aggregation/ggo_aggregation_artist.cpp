#include "ggo_aggregation_artist.h"
#include <ggo_shapes3d.h>
#include <ggo_shape_sampling.h>
#include <ggo_paint.h>
#include <ggo_gaussian_blur.h>
#include <cstring>

namespace 
{
  const int PARTITION_SIZE = 10;
  const int POINS_COUNT = 500000;
  const float DISPLACEMENT = 0.007f;
  
  struct ggo_space_partition
	{
		std::vector<ggo::point3d_float>	_points;
		ggo::aabox3d_float              _aabb;
	};

  //////////////////////////////////////////////////////////////
  ggo::point3d_float create_new_point(float radius)
  {
    return radius * ggo::sphere_uniform_sampling<float>();
  }
  
  //////////////////////////////////////////////////////////////
  void add_point_to_space_partitions(const ggo::point3d_float & point, std::vector<ggo_space_partition> & space_partitions)
  {
    for (auto & space_partition : space_partitions)
    {
      if (space_partition._aabb.is_point_inside(point) == true)
      {
        space_partition._points.push_back(point);
      }
    }
  }
  
  //////////////////////////////////////////////////////////////
  bool point_close_enough(const ggo::point3d_float & point, const std::vector<ggo_space_partition> & space_partitions, float displacement)
  {
    if (point.x() <= -PARTITION_SIZE - DISPLACEMENT || point.x() >= PARTITION_SIZE + DISPLACEMENT ||
        point.y() <= -PARTITION_SIZE - DISPLACEMENT || point.y() >= PARTITION_SIZE + DISPLACEMENT ||
        point.z() <= -PARTITION_SIZE - DISPLACEMENT || point.z() >= PARTITION_SIZE + DISPLACEMENT)
    {
      return false;
    }
    
    int index = ggo::to<int>(point.z()) + PARTITION_SIZE;
    index *= 2 * PARTITION_SIZE + 1;
    index += ggo::to<int>(point.y()) + PARTITION_SIZE;
    index *= 2 * PARTITION_SIZE + 1;
    index += ggo::to<int>(point.x()) + PARTITION_SIZE;

    float squared_disp = displacement * displacement;

    const auto & space_partition = space_partitions[index];

    for (const auto & point2 : space_partition._points)
    {
      float hypot = ggo::hypot(point, point2);
      if (hypot <= squared_disp)
      {
        return true;
      }
    }
    
    return false;
  }
}

namespace ggo
{
  namespace aggregation_artist
  {
    //////////////////////////////////////////////////////////////
    std::vector<ggo::point3d_float> compute_points(float attraction_factor,
                                                   const ggo::vector3d_float rotation_vector,
                                                   float rotation_factor)
    {
      std::cout << "attraction_factor: " << attraction_factor << std::endl;
      std::cout << "rotation_factor: " << rotation_factor << std::endl;
      
      const float displacement = DISPLACEMENT * PARTITION_SIZE;
      
      std::vector<ggo_space_partition>  space_partitions;
    
      // Initialize space partitioning.
      for (int z = -PARTITION_SIZE; z <= PARTITION_SIZE; ++z)
      {
        for (int y = -PARTITION_SIZE; y <= PARTITION_SIZE; ++y)
        {
          for (int x = -PARTITION_SIZE; x <= PARTITION_SIZE; ++x)
          {
            ggo_space_partition space_partition;
    
            space_partition._aabb.x_min() = x - 0.5f - displacement;
            space_partition._aabb.x_max() = x + 0.5f + displacement;
            space_partition._aabb.y_min() = y - 0.5f - displacement;
            space_partition._aabb.y_max() = y + 0.5f + displacement;
            space_partition._aabb.z_min() = z - 0.5f - displacement;
            space_partition._aabb.z_max() = z + 0.5f + displacement;
                
            space_partitions.push_back(space_partition);
          }
        }
      }
      
      // Create points.
      std::vector<ggo::point3d_float> points { ggo::point3d_float(0, 0, 0) };
      add_point_to_space_partitions(points[0], space_partitions);
      
      ggo::vector3d_float unit_rotation_vector(rotation_vector);
      unit_rotation_vector.normalize();

      float radius = 0;
      const float margin = 10 * displacement;
      
      while (points.size() < POINS_COUNT)
      {
        float creation_radius = radius + margin;
        ggo::point3d_float point = create_new_point(creation_radius);
        
        while (true) 
        {
          ggo::vector3d_float unit_vector(point);
          unit_vector.normalize();
          
          // Move the point a bit.
          ggo::vector3d_float random_disp = ggo::sphere_uniform_sampling<float>();
          
          // Rotate the point.
          ggo::vector3d_float rotation = ggo::cross(unit_rotation_vector, unit_vector);

          // Attract the point to the origin.
          ggo::vector3d_float attraction = -unit_vector;
          
          // Add displacements.
          ggo::vector3d_float disp = (1 - rotation_factor - attraction_factor) * random_disp;
          disp += rotation_factor * rotation;
          disp += attraction * attraction_factor;
          GGO_ASSERT(disp.get_hypot() <= 1);
          
          disp *= displacement;
          GGO_ASSERT(disp.get_length() <= displacement);

          point += disp;
          
          // If the point gets away, start from beginning.
          if (point.get_hypot() > ggo::square(creation_radius))
          {
            break;
          }
          
          // If the point is close enough to another one, store it and process another point.
          if (point_close_enough(point, space_partitions, displacement) == true)
          {
            points.push_back(point);
            add_point_to_space_partitions(point, space_partitions);
            
            radius = std::max(radius, point.get_length());
            break;
          }
        }
      }
      
      // Map all the points' coordinates into [-1, 1].
      for (auto & point : points)
      {
        point.x() = ggo::map(point.x(), static_cast<float>(-PARTITION_SIZE), static_cast<float>(PARTITION_SIZE), -1.f, 1.f);
        point.y() = ggo::map(point.y(), static_cast<float>(-PARTITION_SIZE), static_cast<float>(PARTITION_SIZE), -1.f, 1.f);
        point.z() = ggo::map(point.z(), static_cast<float>(-PARTITION_SIZE), static_cast<float>(PARTITION_SIZE), -1.f, 1.f);
      }
    
      return points;
    }
    
    //////////////////////////////////////////////////////////////
    void render(float background, const std::vector<ggo::point3d_float> & points, float angle, uint8_t * buffer, int width, int height)
    {
      // Fill background.
      memset(buffer, ggo::to<int>(255 * background), 3 * width * height);
      
      // At least 2 points otherwise bad things will happen.
      if (points.size() <= 1)
      {
        return;
      }
      
      int min_size = std::min(width, height);
      
      std::vector<ggo::point3d_float> rotated_points(points);
      
      // Rotate points.
      if (angle != 0)
      {
        float cos_tmp = std::cos(angle);
        float sin_tmp = std::sin(angle);
        
        for (auto & point : rotated_points)
        {
          float x = point.x() * cos_tmp - point.z() * sin_tmp;
          float z = point.x() * sin_tmp + point.z() * cos_tmp;
          
          point.x() = x;
          point.z() = z;
        }
      }
      
      // Sort points.
      std::sort(rotated_points.begin(), rotated_points.end(), [](const ggo::point3d_float & p1, const ggo::point3d_float & p2)
      {
        return p1.z() < p2.z();
      });
      
      float z_inf = rotated_points.front().z();
      float z_sup = rotated_points.back().z();

      // Render shadow.
      ggo::gray_image_buffer_uint8 image_buffer(width, height, buffer);
      
      for (const auto & point : rotated_points)
      {
        ggo::pos2f pt = ggo_artist_abc::map_fit(point.xy(), -1, 1, width, height);
        
        // Move the shadow a little bit.
        pt.get<0>() += 0.15f * min_size;
        pt.get<1>() += 0.10f * min_size;

        ggo::paint(image_buffer,
                   std::make_shared<ggo::disc_float>(pt, 0.0015f * min_size),
                   0, 0.5f);
      }
      
      // Gaussian blur.
      float variance = ggo::square(0.1f * min_size);
      
      //fetch_data fetch_data{ ggo::to<uint8_t>(255 * background) };
      //ggo::gaussian_blur_2d<uint8_t, float, float, uint8_t, fetch_data>(buffer, buffer, width, height, variance);
      
      // Render points.
      for (const auto & point : rotated_points)
      {
        ggo::pos2f render_point = ggo_artist_abc::map_fit(point.xy(), -1, 1, width, height);

        float gray = ggo::map(point.z(), z_inf, z_sup, 0.1f, 1.0f);
        float blur = 0.75f + 0.005f * min_size * std::abs(point.z() - 0.75f * z_sup);
        
        ggo::paint(image_buffer,
                   std::make_shared<ggo::disc_float>(render_point, 0.0015f * min_size),
                   gray, 1, std::make_shared<ggo::gray_alpha_blender>(), ggo::blur_pixel_sampler(blur, 24));
      }
      
      // Convert from gray to rgb.
      const uint8_t * src = buffer + height * width - 1;
      uint8_t * dst = buffer + 3 * (height * width - 1);
      
      for (int i = 0; i < width * height; ++i)
      {
        dst[0] = dst[1] = dst[2] = src[0];
        --src;
        dst -= 3;
      }
    }
  }
}
