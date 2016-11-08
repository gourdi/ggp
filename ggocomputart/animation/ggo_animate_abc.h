#ifndef __GGO_ANIMATE_ABC__
#define __GGO_ANIMATE_ABC__

#include <ggo_path_abc.h>

//////////////////////////////////////////////////////////////
class ggo_animate_abc
{
public:

                ggo_animate_abc(int start_offset = 0) : _counter(0), _start_offset(start_offset) {}
  virtual      ~ggo_animate_abc() {}

          bool  update(void * buffer, int width, int height);

          int	  get_counter() const { return _counter; }

          int   get_start_offset() const { return _start_offset; }
          void  set_start_offset(int start_offset) { _start_offset = start_offset; }

private:

  virtual	bool  update(void * buffer, int width, int height, int counter) = 0;

private:

  int	_counter;
  int _start_offset;
};

//////////////////////////////////////////////////////////////
class ggo_position_animate_abc : public ggo_animate_abc
{
public:

              ggo_position_animate_abc(const ggo::pos2f & pos, int start_offset = 0) : ggo_animate_abc(start_offset), _pos(pos) {};

  ggo::pos2f	get_position() const { return _pos; }
  void        set_position(const ggo::pos2f & pos) { _pos = pos; }
  void        set_position(float x, float y) { _pos.get<0>() = x; _pos.get<1>() = y; }

private:

          bool  update(void * buffer, int width, int height, int counter) override;
  virtual bool  update(void * buffer, int width, int height, int counter, const ggo::pos2f & pos) = 0;

private:

  ggo::pos2f	_pos;
};

//////////////////////////////////////////////////////////////
// Takes ownership of the path pointer.
class ggo_path_animate_abc : public ggo_animate_abc
{
public:

                      ggo_path_animate_abc(const ggo::pos2f & pos, ggo_path_abc * path, int start_offset = 0) : ggo_animate_abc(start_offset), _pos(pos), _path(path) {};
  virtual		   			 ~ggo_path_animate_abc() { delete _path; }

          ggo::pos2f  get_position() const { return _pos; };
          void        set_position(const ggo::pos2f & pos) { _pos = pos; };
          void        set_position(float x, float y) { _pos.get<0>() = x; _pos.get<1>() = y; };

private:

          bool			  update(void * buffer, int width, int height, int counter) override;
  virtual	bool        update(void * buffer, int width, int height, int counter, const ggo::pos2f & pos) = 0;

private:

  ggo::pos2f	    _pos;
  ggo_path_abc *	_path;
};

#endif
