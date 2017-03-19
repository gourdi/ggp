#ifndef GGO_DISC_FACTORY
#define GGO_DISC_FACTORY

#include "ShapeFactory.h"

class DiscFactory : public ShapeFactory
{
public:

  DiscFactory(const ggo::color_8u & c) : ShapeFactory(c) {}

private:

  ShapeHandler * OnMouseDown(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) override;
  ShapeHandler * OnMouseUp(Qt::MouseButton button, int x, int y, int width, int height, ggo::canvas & canvas, const ggo::canvas::view & view) override;
  void OnMouseMove(int x, int y, int width, int height, ggo::canvas &, const ggo::canvas::view & view) override;

  enum State
  {
    None,
    SettingRadius,
  };

  State _state = None;
  ggo::canvas::disc * _disc = nullptr;
};

#endif

