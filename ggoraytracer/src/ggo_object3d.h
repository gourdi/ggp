#ifndef __GGO_OBJECT3D__
#define __GGO_OBJECT3D__

#include <ggo_shapes3d.h>
#include <ggo_material_abc.h>
#include <memory>

namespace ggo
{
  class object3d
  {
  public:

                                object3d();
                                object3d(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape);
                                object3d(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape, const ggo::color & color);
                                object3d(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape, std::shared_ptr<const ggo::material_abc> material);

    ggo::pos3f                  sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const;
    ggo::ray3d_float            sample_ray(float random_variable1, float random_variable2) const;

    ggo::ray3d_float            get_reflected_ray(const ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal) const;
    ggo::ray3d_float            sample_reflection_ray(const ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal, float random_variable1, float random_variable2) const;

    void                        set_roughness(float roughness) { _roughness = ggo::clamp<float>(roughness, 0, 1); }
    float                       get_roughness() const { return _roughness; }

    float						            get_reflection_factor() const { return _reflection_factor; }
    void						            set_reflection_factor(float v) { _reflection_factor = ggo::clamp<float>(v, 0, 1); }

    void                        set_transparent(bool transparent) { _transparent = transparent; }
    bool						            is_transparent() const { return _transparent; }

    void                        set_density(float density) { _density = density; }
    float                       get_density() const { return _density; }

    void                        set_discard_basis(bool discard) { _discard_basis = discard; }

    ggo::basis3d_float &        basis() { return _basis; }
    const ggo::basis3d_float &  basis() const { return _basis; }

    const ggo::material_abc *   material() const { return _material.get(); }

    ggo::color                  get_color(const ggo::pos3f & world_pos) const;

    const ggo::color &          get_emissive_color() const { return _emissive_color; }
    void                        set_emissive_color(const ggo::color & color) { _emissive_color = color; }

    const ggo::color &          get_transmission_color() const { return _transmission_color; }
    void                        set_transmission_color(const ggo::color & color) { _transmission_color = color; }

    bool	                      intersect_ray(const ggo::ray3d_float & ray, float & dist, ggo::ray3d_float & local_normal, ggo::ray3d_float & world_normal) const;

    const ggo::object3d *       handle_self_intersection(ggo::ray3d_float & ray, bool inside) const;

    // Return false if the ray is reflected (below incidence angle) or if we have reach recursion depth.
    bool                        transmit_ray(ggo::ray3d_float & ray, ggo::ray3d_float world_normal, int & depth) const;

    void                        set_phong_factor(float phong_factor) { _phong_factor = phong_factor; }
    float                       get_phong_factor() const { return _phong_factor; }
    void                        set_phong_shininess(float phong_shininess) { _phong_shininess = phong_shininess; }
    float                       get_phong_shininess() const { return _phong_shininess; }

  private:

    ggo::basis3d_float                                        _basis;
    bool                                                      _discard_basis = false;
    float				                                              _reflection_factor = 0;
    bool                                                      _transparent = false;
    float                                                     _density = 1;
    float                                                     _roughness = 0;
    float                                                     _phong_factor = 0;
    float                                                     _phong_shininess = 0;
    ggo::color                                                _transmission_color = ggo::color::WHITE;
    ggo::color                                                _emissive_color = ggo::color::BLACK;
    std::shared_ptr<const ggo::raytracable_shape3d_abc_float> _shape;
    std::shared_ptr<const ggo::material_abc>                  _material;
  };

  std::shared_ptr<ggo::object3d>  create_point_light(const ggo::color & color, const ggo::pos3f & pos);
  std::shared_ptr<ggo::object3d>  create_sphere_light(const ggo::color & color, float radius, const ggo::pos3f & pos);
}

#endif
