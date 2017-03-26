#ifndef __GGO_CANVAS__
#define __GGO_CANVAS__

#include <ggo_pixel_buffer.h>
#include <ggo_multi_shape_paint.h>
#include <memory>

namespace tinyxml2
{
  class XMLElement;
  class XMLDocument;
}

namespace ggo
{
  class canvas
  {
  public:

    enum class main_direction
    {
      horizontal,
      vertical
    };

    struct view
    {
      view(const ggo::pos2f & center, float size, main_direction main_dir) : _center(center), _size(size), _main_direction(main_dir) {}

      ggo::pos2f      _center;
      float           _size;
      main_direction  _main_direction;
    };

  ///////////////////////////////////////////////////////////////////
  // Shapes.
  public:

    // Shapes interface.
    class shape_abc
    {
    public:

      virtual const ggo::paintable_shape2d_abc<float> *   get_shape() const = 0;
      virtual ggo::paintable_shape2d_abc<float> *         create_render_shape(const view & view, int render_width, int render_height, bool vertical_flip) const = 0;
      
      std::unique_ptr<ggo::paintable_shape2d_abc<float>>  make_unique_render_shape(const view & view, int render_width, int render_height, bool vertical_flip) const;

      virtual tinyxml2::XMLElement *  create_xml_element(tinyxml2::XMLDocument * document) const = 0;

      ggo::color_8u _color;
    };

    // Disc.
    class disc : public shape_abc
    {
    public:

      const ggo::paintable_shape2d_abc<float> * get_shape() const override { return &_disc; }
      ggo::paintable_shape2d_abc<float> *       create_render_shape(const view & view, int render_width, int render_height, bool vertical_flip) const override { return create_render_disc(view, render_width, render_height, vertical_flip); }

      void  set_from_render_points(const ggo::pos2f & p1, const ggo::pos2f & p2, const view & view, int render_width, int render_height, bool vertical_flip);
      void  set_from_render_disc(const ggo::disc_float & disc, const view & view, int render_width, int render_height, bool vertical_flip);

      ggo::disc_float &       get_disc() { return _disc; }
      const ggo::disc_float & get_disc() const { return _disc; }
      ggo::disc_float *       create_render_disc(const view & view, int render_width, int render_height, bool vertical_flip) const;

      tinyxml2::XMLElement *  create_xml_element(tinyxml2::XMLDocument * document) const override;

    private:

      ggo::disc_float _disc;
    };

    // Polygon.
    class polygon : public shape_abc
    {
    public:

      const ggo::paintable_shape2d_abc<float> * get_shape() const override { return &_polygon; }
      ggo::paintable_shape2d_abc<float> *       create_render_shape(const view & view, int render_width, int render_height, bool vertical_flip) const override { return create_render_polygon(view, render_width, render_height, vertical_flip); }

      void  update_render_point(int index, const ggo::pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip);
      void  set_from_render_polygon(const ggo::polygon2d_float & polygon, const view & view, int render_width, int render_height, bool vertical_flip);

      ggo::polygon2d_float &        get_polygon() { return _polygon; }
      const ggo::polygon2d_float &  get_polygon() const { return _polygon; }
      ggo::polygon2d_float *        create_render_polygon(const view & view, int render_width, int render_height, bool vertical_flip) const;

      tinyxml2::XMLElement *  create_xml_element(tinyxml2::XMLDocument * document) const override;

    private:

      ggo::polygon2d_float _polygon;
    };

  ///////////////////////////////////////////////////////////////////
  // Methods.
  public:

    canvas() = default;

    // Rendering.
    void render(void * buffer, const view & view, int width, int height, int line_byte_step, pixel_buffer_format pbf) const;
    
    const ggo::color_8u & background_color() const { return _background_color; }
    ggo::color_8u &       background_color()       { return _background_color; }

    // Shapes container.
    disc *    create_disc();
    polygon * create_polygon();

    void remove_shape(const ggo::paintable_shape2d_abc<float> * shape);

    // Frame conversion.
    static float      from_view_to_render(float dist, float view_size, main_direction main_dir, int render_width, int render_height);
    static float      from_render_to_view(float dist, float view_size, main_direction main_dir, int render_width, int render_height);

    static ggo::pos2f from_view_to_render(const ggo::pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip);
    static ggo::pos2f from_render_to_view(const ggo::pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip);

    static ggo::pos2i from_view_to_render_pixel(const ggo::pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip);
    static ggo::pos2f from_render_pixel_to_view(const ggo::pos2i & p, const view & view, int render_width, int render_height, bool vertical_flip);

    // I/O
    struct load_delegate
    {
      virtual void on_create_disc(disc * disc) = 0;
      virtual void on_create_polygon(polygon * polygon) = 0;
    };

                  canvas(const std::string & xml, load_delegate * delegate = nullptr);

    void          save(const char * filename) const;
    void          load(const char * filename, load_delegate * delegate);

    std::string   to_string() const;
    void          from_string(const std::string & xml, load_delegate * delegate);

  private:

    // I/O private functions.
    std::unique_ptr<tinyxml2::XMLDocument> create_xml_document() const;
    void from_xml_document(const tinyxml2::XMLDocument & document, load_delegate * delegate);
    canvas(const tinyxml2::XMLDocument & document, load_delegate * delegate);

    canvas::shape_abc * create_disc(const tinyxml2::XMLElement & shape_element, load_delegate * delegate);
    canvas::shape_abc * create_polygon(const tinyxml2::XMLElement & shape_element, load_delegate * delegate);

  private:

    ggo::color_8u                           _background_color = ggo::white_8u();
    std::vector<std::unique_ptr<shape_abc>> _shapes;
  };
}

#endif

