#ifndef __GGO_SCENE__
#define __GGO_SCENE__

#include <ggo_object3d.h>
#include <ggo_fog_abc.h>
#include <ggo_background3d_abc.h>
#include <memory>

namespace ggo
{
  class scene
  {
  public:

    scene() = default;
    scene(const std::vector<std::shared_ptr<const ggo::object3d>> & objects,
          std::shared_ptr<const ggo::background3d_abc> background,
          const ggo::color & ambient_color,
          std::shared_ptr<const ggo::fog_abc> fog);

    const std::vector<std::shared_ptr<const ggo::object3d>> & objects() const { return _objects; }
    const std::vector<std::shared_ptr<const ggo::object3d>> & lights() const { return _lights; }

    const ggo::fog_abc *          fog() const { return _fog.get(); }
    const ggo::background3d_abc & background() const { return *_background; }
    const ggo::color &            ambient_color() const { return _ambient_color; }

  private:

    std::vector<std::shared_ptr<const ggo::object3d>>	_objects;
    std::vector<std::shared_ptr<const ggo::object3d>> _lights;
    std::shared_ptr<const ggo::background3d_abc>      _background;
    ggo::color                                        _ambient_color;
    std::shared_ptr<const ggo::fog_abc>               _fog;
  };
}

namespace ggo
{
  class scene_builder
  {
  public:

                    scene_builder(std::shared_ptr<const ggo::background3d_abc> background);

    std::shared_ptr<ggo::object3d>  add_point_light(const ggo::color & color, const ggo::pos3f & pos);
    std::shared_ptr<ggo::object3d>  add_sphere_light(const ggo::color & color, float radius, const ggo::pos3f & pos);

    void                            add_object(std::shared_ptr<ggo::object3d> object, bool discard_basis);
    std::shared_ptr<ggo::object3d>  add_object(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape, const ggo::color & color, bool discard_basis);
    std::shared_ptr<ggo::object3d>  add_object(std::shared_ptr<const ggo::raytracable_shape3d_abc_float> shape, std::shared_ptr<const ggo::material_abc> material, bool discard_basis);

    void                            set_fog(std::shared_ptr<const ggo::fog_abc> fog) { _fog = fog; }
    void                            set_ambient_color(const ggo::color & ambient_color) { _ambient_color = ambient_color; }

    scene                           build_scene() const { return ggo::scene(_objects, _background, _ambient_color, _fog); }

  private:

    std::vector<std::shared_ptr<const ggo::object3d>> _objects;
    std::shared_ptr<const ggo::background3d_abc>      _background;
    ggo::color                                        _ambient_color = ggo::color::BLACK;
    std::shared_ptr<const ggo::fog_abc>               _fog;
  };
}

#endif

