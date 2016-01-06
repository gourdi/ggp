struct ggo_ray3d
{
    float3  pos;
    float3  dir;
};

struct ggo_render_ray
{
	struct ggo_ray3d ray;
	float weight;
  int exclude_object;
};

struct ggo_hit_info
{
	float dist;
	struct ggo_ray3d normal;
	float3 color;
	float reflexion_factor;
  int object_index;
};

bool ggo_sphere_intersect_ray(__global const float * scene_data, const struct ggo_ray3d * ray, struct ggo_ray3d * normal, float * dist)
{
	// Build the quadratic and solve it.
	const float3 sphere_center = (float3)(scene_data[0], scene_data[1], scene_data[2]);
	const float3 diff = ray->pos - sphere_center;
	
	const float deg1 = dot(ray->dir, diff);
	const float deg0 = dot(diff, diff) - scene_data[3] * scene_data[3];
	
	// Solve it.
	const float d = deg1 * deg1 - deg0;
	if (d <= 0)
	{
		return false;
	}

	*dist = -deg1 - sqrt(d);
	
	// The lower intersection is behind the ray's origin.
	if (*dist <= 0)
	{
		return false;
	}

	// The intersection is valid, build the normal.
	normal->pos = ray->pos + *dist * ray->dir;
	normal->dir = normalize(normal->pos - sphere_center);

	return true;
}

bool ggo_plane_intersect_ray(__global const float * scene_data, const struct ggo_ray3d * ray, struct ggo_ray3d * normal, float * dist)
{
	const float3 plane_normal = (float3)(scene_data[0], scene_data[1], scene_data[2]);
	const float den = dot(plane_normal, ray->dir);
	
	if (fabs(den) < 0.0001f)
	{
		return false;
	}
	
	const float num = dot(plane_normal, ray->pos) + scene_data[3];
	*dist = -num / den;
	
	if (*dist <= 0)
	{
		return false;
	}
	
	normal->pos = ray->pos + *dist * ray->dir;
	normal->dir = normalize(plane_normal);
	if (den > 0)
	{
		normal->dir *= -1;
	}

	return true;
}

bool ggo_hittest(__global const float * scene_data,
				         uint scene_data_size,
				         const struct ggo_ray3d * ray,
				         struct ggo_hit_info * hit_info,
                 int exclude_object)
{
	bool result = false;
	
	hit_info->dist = FLT_MAX;

	struct ggo_ray3d cur_normal;
	float dist_cur;
  int object_index = 0;

	// Plane.
	if (exclude_object != object_index && 
      ggo_plane_intersect_ray(scene_data, ray, &cur_normal, &dist_cur))
	{
		hit_info->dist = dist_cur;
		hit_info->normal = cur_normal;
		hit_info->color = (float3)(scene_data[4], scene_data[5], scene_data[6]);
		hit_info->reflexion_factor = scene_data[7];
    hit_info->object_index = object_index;

		result = true;
	}

  scene_data += 8;
	scene_data_size -= 8;
  object_index += 1;

	// Spheres.
	while (scene_data_size > 0)
	{
		if (exclude_object != object_index && 
        ggo_sphere_intersect_ray(scene_data, ray, &cur_normal, &dist_cur) &&
			  dist_cur < hit_info->dist)
		{
			hit_info->dist = dist_cur;
			hit_info->normal = cur_normal;
			hit_info->color = (float3)(scene_data[4], scene_data[5], scene_data[6]);
			hit_info->reflexion_factor = scene_data[7];
      hit_info->object_index = object_index;

			result = true;
		}

    scene_data += 8;
		scene_data_size -= 8;
    object_index += 1;
	}

	return result;
}

void ggo_raytrace(__global const float * scene_data,
				          uint scene_data_size,
				          __global const float * lights,
				          uint lights_count,
				          const struct ggo_render_ray * render_ray,
				          struct ggo_render_ray * reflected_ray,
				          float3 * color)
{
	struct ggo_hit_info hit_info;

	reflected_ray->weight = 0;

	if (ggo_hittest(scene_data, scene_data_size, &render_ray->ray, &hit_info, render_ray->exclude_object))
	{
		float3 diffuse_color = (float3)(0, 0, 0);

		// Process each light.
	  while (lights_count > 0)
		{
			float3 diff = (float3)(lights[0], lights[1], lights[2]) - hit_info.normal.pos;

			// Check shadow.
			struct ggo_ray3d light_ray;
			light_ray.pos = hit_info.normal.pos;
			light_ray.dir = normalize(diff);
			
			struct ggo_hit_info light_hit_info;
			bool shadow = ggo_hittest(scene_data, scene_data_size, &light_ray, &light_hit_info, hit_info.object_index);

			// We need to make sure the light is behind the object.
			if (shadow)
			{
				shadow = light_hit_info.dist * light_hit_info.dist < dot(diff, diff);
			}

			if (!shadow)
			{
				diffuse_color +=  max(0.f, dot(hit_info.normal.dir, light_ray.dir)) *
									        (float3)(lights[3], lights[4], lights[5]) * // Light color
									        hit_info.color;
			}

			lights += 6;
      --lights_count;
		}

    *color += (1 - hit_info.reflexion_factor) * render_ray->weight * diffuse_color;

		// Reflexion.
		if (hit_info.reflexion_factor > 0)
		{
			reflected_ray->ray.pos = hit_info.normal.pos;
			reflected_ray->ray.dir = normalize(render_ray->ray.dir - 2 * dot(hit_info.normal.dir, render_ray->ray.dir) * hit_info.normal.dir);
			reflected_ray->weight = hit_info.reflexion_factor * render_ray->weight;
      reflected_ray->exclude_object = hit_info.object_index;
		}
	}
}

#define camera_pos		      ((float3)(camera[0], camera[1], camera[2]))
#define camera_basis_x	    ((float3)(camera[3], camera[4], camera[5]))
#define camera_basis_y	    ((float3)(camera[6], camera[7], camera[8]))
#define center_focus_point	((float3)(camera[9], camera[10], camera[11]))
#define opti    		        camera[12]

__kernel void ggo_kernel(__write_only image2d_t image,
						             __global const float * scene_data,
						             uint scene_data_size,
						             __global const float * camera,
						             __global const float * lights,
						             uint lights_count)
{
	const int x = get_global_id(0);
	const int y = get_global_id(1);

	const float width_f = get_image_width(image);
	const float height_f = get_image_height(image);
	const float x_f = x + 0.5f - 0.5f * height_f - 0.5f * (width_f - height_f);
	const float y_f = y + 0.5f - 0.5f * height_f;

  const float3 focus_point = center_focus_point + camera_basis_x * (x_f * opti) + camera_basis_y * (y_f * opti);

  struct ggo_render_ray render_ray1;
  render_ray1.ray.pos = camera_pos;
  render_ray1.ray.dir = normalize(focus_point - render_ray1.ray.pos);
  render_ray1.weight = 1;
  render_ray1.exclude_object = -1;

  struct ggo_render_ray render_ray2;
  float3 color = (float3)(0, 0, 0);
  ggo_raytrace(scene_data, scene_data_size, lights, lights_count, &render_ray1, &render_ray2, &color);
  ggo_raytrace(scene_data, scene_data_size, lights, lights_count, &render_ray2, &render_ray1, &color);
  ggo_raytrace(scene_data, scene_data_size, lights, lights_count, &render_ray1, &render_ray2, &color);

  write_imagef(image, (int2)(x, y), (float4)(color, 1));
}
