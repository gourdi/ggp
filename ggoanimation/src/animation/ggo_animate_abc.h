#ifndef __GGO_ANIMATE_ABC__
#define __GGO_ANIMATE_ABC__

#include <animation/ggo_path_abc.h>
#include <2d/ggo_image.h>

//////////////////////////////////////////////////////////////
namespace ggo
{
  class animate_abc
  {
  public:

    animate_abc(int start_offset = 0) : _frame_index(-1), _start_offset(start_offset) {}
    virtual      ~animate_abc() {}

    bool  update();
    void  render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::rect_int & clipping) const;

    int	  get_frame_index() const { return _frame_index; }

    int   get_start_offset() const { return _start_offset; }
    void  set_start_offset(int start_offset) { _start_offset = start_offset; }

  private:

    virtual	bool  update(int frame_index) = 0;
    virtual	void  render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::rect_int & clipping, int frame_index) const = 0;

  private:

    int	_frame_index;
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

            bool  update(int frame_index) override;
    virtual bool  update(int frame_index, const ggo::pos2f & pos) = 0;

            void  render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::rect_int & clipping, int frame_index) const override;
    virtual void  render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::rect_int & clipping, int frame_index, const ggo::pos2f & pos) const = 0;

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

                        path_animate_abc(const ggo::pos2f & pos, ggo::path_abc * path, int start_offset = 0) : ggo::animate_abc(start_offset), _start_pos(pos), _path(path) {};
    virtual		   			 ~path_animate_abc() { delete _path; }

            ggo::pos2f  get_position() const { return _start_pos; };
            void        set_position(const ggo::pos2f & pos) { _start_pos = pos; };
            void        set_position(float x, float y) { _start_pos.x() = x; _start_pos.y() = y; };

  private:

            bool  update(int frame_index) override;
    virtual bool  update(int frame_index, const ggo::pos2f & pos) = 0;

            void  render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::rect_int & clipping, int frame_index) const override;
    virtual void  render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::rect_int & clipping, int frame_index, const ggo::pos2f & pos) const = 0;

  private:

    ggo::pos2f	    _start_pos;
    ggo::pos2f	    _cur_pos;
    ggo::path_abc *	_path;
  };
}

#endif
