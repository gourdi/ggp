#ifndef __GGO_ANIMATE_ABC__
#define __GGO_ANIMATE_ABC__

#include <ggo_path_abc.h>
#include <ggo_pixel_buffer.h>

//////////////////////////////////////////////////////////////
namespace ggo
{
  class animate_abc
  {
  public:

    animate_abc(int start_offset = 0) : _counter(0), _start_offset(start_offset) {}
    virtual      ~animate_abc() {}

    bool  update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf);

    int	  get_counter() const { return _counter; }

    int   get_start_offset() const { return _start_offset; }
    void  set_start_offset(int start_offset) { _start_offset = start_offset; }

  private:

    virtual	bool  update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter) = 0;

  private:

    int	_counter;
    int _start_offset;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  class position_animate_abc : public animate_abc
  {
  public:

                position_animate_abc(const ggo::pos2f & pos, int start_offset = 0) : animate_abc(start_offset), _pos(pos) {};

    ggo::pos2f	get_position() const { return _pos; }
    void        set_position(const ggo::pos2f & pos) { _pos = pos; }
    void        set_position(float x, float y) { _pos.get<0>() = x; _pos.get<1>() = y; }

  private:

            bool  update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter) override;
    virtual bool  update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter, const ggo::pos2f & pos) = 0;

  private:

    ggo::pos2f	_pos;
  };
}

//////////////////////////////////////////////////////////////
// Takes ownership of the path pointer.
namespace ggo
{
  class path_animate_abc : public animate_abc
  {
  public:

                        path_animate_abc(const ggo::pos2f & pos, ggo::path_abc * path, int start_offset = 0) : ggo::animate_abc(start_offset), _pos(pos), _path(path) {};
    virtual		   			 ~path_animate_abc() { delete _path; }

            ggo::pos2f  get_position() const { return _pos; };
            void        set_position(const ggo::pos2f & pos) { _pos = pos; };
            void        set_position(float x, float y) { _pos.get<0>() = x; _pos.get<1>() = y; };

  private:

            bool			  update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter) override;
    virtual	bool        update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter, const ggo::pos2f & pos) = 0;

  private:

    ggo::pos2f	    _pos;
    ggo::path_abc *	_path;
  };
}

#endif
