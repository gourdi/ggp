#ifndef GGO_DISC_FACTORY
#define GGO_DISC_FACTORY

#include "ShapeFactory.h"

class DiscFactory : public ShapeFactory
{
private:

  void OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) override;
  void OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) override;
  bool OnMouseMove(int x, int y, int width, int height, ggo::canvas &, const ggo::canvas::view & view) override;

  enum State
  {
    None,
    SettingRadius,
  };

  State _state = None;
  ggo::disc_float * _disc = nullptr;
};

#endif

