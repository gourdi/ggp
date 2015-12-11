//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T, bool double_sided = false>
  class face3d : public raytracable_shape3d_abc<T>
  {
  public:

                      face3d(const vertex<T> & v1, const vertex<T> & v2, const vertex<T> & v3) : _v1(v1), _v2(v2), _v3(v3) { }

    const vertex<T> & v1() const { return _v1; }
    const vertex<T> & v2() const { return _v2; }
    const vertex<T> & v3() const { return _v3; }

    bool              intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const override;
    bool              is_convex() const override { return true; }

    std::string       desc() const override;
      
  private:

    vertex<T> _v1;
    vertex<T> _v2;
    vertex<T> _v3;
  };
}