#ifndef __GGO_VIVELLE_NAVIGATION_TOOL__
#define __GGO_VIVELLE_NAVIGATION_TOOL__

#include <ToolAbc.h>

class NavigationTool : public ToolAbc
{
public:

  NavigationTool(ImageWidget & imageWidget);

private:

  void doMousePressEvent(const ggo::pos2i & mousePos) override;
  void doMouseReleaseEvent(const ggo::pos2i & mousePos) override;
  void doMouseMoveEvent(const ggo::pos2i & mousePos) override;
  void doWheelEvent(const ggo::pos2i & mousePos, int angle) override;
};

#endif

