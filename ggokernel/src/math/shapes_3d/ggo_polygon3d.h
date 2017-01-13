//////////////////////////////////////////////////////////////
// Each vertex of the polygon is associated with a normal so that 
// when intersecting rays, normal is smoothly interpolated.

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class polygon3d : public raytracable_shape3d_abc<T>
  {
  public:

    struct face
    {
      face(int v1, int v2, int v3) : _v1(v1), _v2(v2), _v3(v3) {};

      int _v1;
      int _v2;
      int _v3;
    };

  public:

                    polygon3d() {};
                    polygon3d(const std::vector<vertex<T>> & vertices, const std::vector<face> & faces);
        
    bool            intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const override;
    bool            is_convex() const override { return false; } // To be improved...
    
    std::ostream &  operator<<(std::ostream & os) const override;

    // Factories.
    static  polygon3d<T>  create_sphere(T radius, int horz_steps, int vert_steps);

  private:

    std::vector<vertex<T>>  _vertices;
    std::vector<face>       _faces;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using polygon3d_double  = polygon3d<double>;
  using polygon3d_float   = polygon3d<float>;
}
