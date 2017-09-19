#include "ggo_cumbia_artist.h"
#include <ggo_object3d.h>
#include <ggo_renderer_abc.h>

namespace
{
  constexpr float camera_dist = 5.f;

  class ggo_boxes_tree_shape : public ggo::raytracable_shape3d_abc_float
  {
  public:

    ggo_boxes_tree_shape(ggo::tree<ggo::aabox3d_float> * tree) : _tree(tree) {}

    bool            intersect_ray(const ggo::ray3d<float> & ray, float & dist, ggo::ray3d<float> & normal) const override;
    bool            is_convex() const override { return false; }
    std::ostream &  operator<<(std::ostream & os) const override { os << "cumbia"; return os; }

    ggo::tree<ggo::aabox3d_float> * _tree;
  };
}

namespace
{
  //////////////////////////////////////////////////////////////
  bool tree_intersect_ray(const ggo::tree<ggo::aabox3d_float> & tree, const ggo::ray3d<float> & ray, float & dist, ggo::ray3d<float> & normal)
  {
    const ggo::aabox3d_float & box = tree.data();
    bool intersect = box.intersect_ray(ray, dist, normal);
    
    if (intersect == false)
    {
      return false;
    }
    
    if (tree.is_leaf() == true)
    {
      return intersect;
    }
    else
    {
      // Recurse.
      dist = std::numeric_limits<float>::max();
      intersect = false;
          
      for (const auto & subtree : tree.subtrees())
      {
        ggo::ray3d<float> current_normal;
        float current_dist = 0;
              
        if ((tree_intersect_ray(subtree, ray, current_dist, current_normal) == true) && (current_dist < dist))
        {
          intersect = true;
          dist = current_dist;
          normal = current_normal;
        }
      }
      
      return intersect;
    }
  }
  
  //////////////////////////////////////////////////////////////
  bool ggo_boxes_tree_shape::intersect_ray(const ggo::ray3d<float> & ray, float & dist, ggo::ray3d<float> & normal) const
  {
    return tree_intersect_ray(*_tree, ray, dist, normal);
  }

  //////////////////////////////////////////////////////////////
  void split_box27(const ggo::aabox3d_float & box, std::vector<ggo::aabox3d_float> & boxes)
  {
    float x1 = ggo::rand<float>(box.x_min() + 2 / 9.f * box.get_size_x(), box.x_min() + 4 / 9.f * box.get_size_x());
    float y1 = ggo::rand<float>(box.y_min() + 2 / 9.f * box.get_size_y(), box.y_min() + 4 / 9.f * box.get_size_y());
    float z1 = ggo::rand<float>(box.z_min() + 2 / 9.f * box.get_size_z(), box.z_min() + 4 / 9.f * box.get_size_z());

    float x2 = ggo::rand<float>(box.x_min() + 5 / 9.f * box.get_size_x(), box.x_min() + 7 / 9.f * box.get_size_x());
    float y2 = ggo::rand<float>(box.y_min() + 5 / 9.f * box.get_size_y(), box.y_min() + 7 / 9.f * box.get_size_y());
    float z2 = ggo::rand<float>(box.z_min() + 5 / 9.f * box.get_size_z(), box.z_min() + 7 / 9.f * box.get_size_z());

    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, box.y_min(), y1, box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, box.y_min(), y1, z1, z2));
    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, box.y_min(), y1, z2, box.z_max()));
    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, y1, y2, box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, y1, y2, z1, z2));
    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, y1, y2, z2, box.z_max()));
    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, y2, box.y_max(), box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, y2, box.y_max(), z1, z2));
    boxes.push_back(ggo::aabox3d_float(box.x_min(), x1, y2, box.y_max(), z2, box.z_max()));

    boxes.push_back(ggo::aabox3d_float(x1, x2, box.y_min(), y1, box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(x1, x2, box.y_min(), y1, z1, z2));
    boxes.push_back(ggo::aabox3d_float(x1, x2, box.y_min(), y1, z2, box.z_max()));
    boxes.push_back(ggo::aabox3d_float(x1, x2, y1, y2, box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(x1, x2, y1, y2, z1, z2));
    boxes.push_back(ggo::aabox3d_float(x1, x2, y1, y2, z2, box.z_max()));
    boxes.push_back(ggo::aabox3d_float(x1, x2, y2, box.y_max(), box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(x1, x2, y2, box.y_max(), z1, z2));
    boxes.push_back(ggo::aabox3d_float(x1, x2, y2, box.y_max(), z2, box.z_max()));

    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), box.y_min(), y1, box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), box.y_min(), y1, z1, z2));
    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), box.y_min(), y1, z2, box.z_max()));
    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), y1, y2, box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), y1, y2, z1, z2));
    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), y1, y2, z2, box.z_max()));
    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), y2, box.y_max(), box.z_min(), z1));
    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), y2, box.y_max(), z1, z2));
    boxes.push_back(ggo::aabox3d_float(x2, box.x_max(), y2, box.y_max(), z2, box.z_max()));
  }
}

//////////////////////////////////////////////////////////////
ggo::cumbia_artist::cumbia_artist()
:
_boxes_tree(nullptr)
{
}

//////////////////////////////////////////////////////////////
ggo::cumbia_artist::~cumbia_artist()
{
	delete _boxes_tree;
}

//////////////////////////////////////////////////////////////
std::vector<ggo::tree<ggo::aabox3d_float> *> ggo::cumbia_artist::init_common(ggo::basis3d_float & camera_basis, float & aperture, int boxes_count)
{
  float dimension = ggo::rand<float>(0.2f, 0.5f);
	float search_ratio = ggo::rand<float>(0, 0.3f);
	float reflection_factor = ggo::rand<float>(0.2f, 0.4f);
	
	delete _boxes_tree;
	_boxes_tree = new ggo::tree<ggo::aabox3d_float>(ggo::aabox3d_float(-1, 1, -1, 1, -1, 1));

	std::vector<ggo::tree<ggo::aabox3d_float> *> leaves;
	leaves.push_back(_boxes_tree);

	// Create boxes tree.
	bool done = false;
	while (done == false)
	{
		// Pick up a leaf.
    int sup = ggo::round_to<int>(search_ratio * (static_cast<int>(leaves.size()) - 1));
		int index = ggo::rand<int>(0, sup);
		ggo::tree<ggo::aabox3d_float> * leaf = leaves[index];
		leaves.erase(leaves.begin() + index);

		// Split it.
		std::vector<ggo::aabox3d_float> new_boxes;
		split_box27(leaf->data(), new_boxes);

		// Insert new boxes in the tree, and in the leaves' list.
		for (ggo::aabox3d_float & new_box : new_boxes)
		{
			float d = new_box.get_center().get_length();
			if (d < dimension + ggo::rand<float>())
			{
				std::vector<ggo::tree<ggo::aabox3d_float> *>::iterator insert_it;

				float size_max1 = std::max(new_box.get_size_x(), std::max(new_box.get_size_y(), new_box.get_size_z()));
				for (insert_it = leaves.begin(); insert_it != leaves.end(); ++insert_it)
				{
					const ggo::aabox3d_float & box2 = (*insert_it)->data();
					float size_max2 = std::max(box2.get_size_x(), std::max(box2.get_size_y(), box2.get_size_z()));
					if (size_max1 >= size_max2)
					{
						break;
					}
				}
				auto & sub_leaf = leaf->create_leaf(new_box);
				leaves.insert(insert_it, &sub_leaf);
			}
			if (leaves.size() > boxes_count)
			{
				done = true;
				break;
			}
		}
	}

	// Set up the raytracer, and add the current tree.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color_32f(ggo::rand<float>(0.25f, 0.75f))));

	auto boxes_tree_shape = std::make_shared<ggo_boxes_tree_shape>(_boxes_tree);
  auto box_tree_object = scene_builder.add_object(boxes_tree_shape, ggo::white<ggo::color_32f>(), true);

	// The camera.
	camera_basis.reset();
	camera_basis.set_pos(0, 0, camera_dist);
	camera_basis.rotate(ggo::ray3d_float::O_X(), 1.2f);
	camera_basis.rotate(ggo::ray3d_float::O_Z(), ggo::rand<float>(0, 2 * ggo::pi<float>()));
	aperture = 0.3f;

	// The lights.
  float angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());
  ggo::pos3f light_pos1 { camera_dist * std::cos(angle_offset),                            camera_dist * std::sin(angle_offset),                            0.f };
  ggo::pos3f light_pos2 { camera_dist * std::cos(angle_offset + 2 * ggo::pi<float>() / 3), camera_dist * std::sin(angle_offset + 2 * ggo::pi<float>() / 3), 0.f };
  ggo::pos3f light_pos3 { camera_dist * std::cos(angle_offset - 2 * ggo::pi<float>() / 3), camera_dist * std::sin(angle_offset - 2 * ggo::pi<float>() / 3), 0.f };
  ggo::pos3f light_pos4 { 0.f, 0.f, camera_dist };

	scene_builder.add_sphere_light(ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), 1.f, 0.75f), 0.1f, light_pos1);
  scene_builder.add_sphere_light(ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), 1.f, 0.75f), 0.1f, light_pos2);
  scene_builder.add_sphere_light(ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), 1.f, 0.75f), 0.1f, light_pos3);
  scene_builder.add_sphere_light(ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), 1.f, 0.75f), 0.1f, light_pos4);

  _scene = scene_builder.build_scene();

	std::cout << "dimension: " << dimension << std::endl;
	std::cout << "search_ratio: " << search_ratio << std::endl;
  
  return leaves;
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_artist::init(ggo::basis3d_float & camera_basis, float & aperture, int boxes_count)
{
  init_common(camera_basis, aperture, boxes_count);
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_artist::init(ggo::basis3d_float & camera_basis, float & aperture, float & depth_of_field, float depth_of_field_factor, int boxes_count)
{
  auto leaves = init_common(camera_basis, aperture, boxes_count);
  
	depth_of_field_factor = ggo::rand<float>(0.01f, 0.02f);
	depth_of_field = camera_dist;
	for (auto & leaf : leaves)
	{
		depth_of_field = std::min(depth_of_field, ggo::distance(leaf->data().get_center(), camera_basis.pos()));
	}
  
  std::cout << "depth_of_field: " << depth_of_field << std::endl;
	std::cout << "depth_of_field_factor: " << depth_of_field_factor << std::endl;
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_artist::render_bitmap(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, ggo::renderer_abc & renderer) const
{
	ggo::raytrace_params raytrace_params;
	raytrace_params._depth = 2;

	renderer.render(buffer, width, height, line_step, pbf, _scene, raytrace_params);
}

