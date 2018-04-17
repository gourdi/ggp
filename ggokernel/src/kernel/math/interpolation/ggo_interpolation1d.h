#ifndef __GGO_INTERPOLATE_1D__
#define __GGO_INTERPOLATE_1D__

namespace ggo
{
  template <typename t_x, typename t_y>
  t_y linear_interpolation(t_x x0, t_y y0, t_x x1, t_y y1, t_x x)
  {
    t_x det = x0 - x1;
    if (std::abs(det) < t_x(0.0000001))
    {
      return (y0 + y1) / t_x(2);
    }
    
    t_y a = y0 - y1;
    t_y b = x0 * y1 - x1 * y0;

    return (a * x + b) / det;
  }

  template <typename t_x, typename t_y>
  t_y cubic_interpolation(t_x x0, t_y y0, t_x x1, t_y y1, t_x x2, t_y y2, t_x x3, t_y y3, t_x x)
  {
    if (std::abs(x2 - x0) <= t_x(0.0000001))
    {
      return (y1 + y2) / t_x(2);
    }
    
    if (std::abs(x3 - x1) <= t_x(0.0000001))
    {
      return (y1 + y2) / t_x(2);
    }

    t_x x12 = x1 * x1;
    t_x x13 = x1 * x12;
    t_x x22 = x2 * x2;
    t_x x23 = x2 * x22;
    
    t_x det = 3 * (x1 * x22 - x12 * x2) + x13 - x23;
    if (std::abs(det) <= t_x(0.0000001))
    {
      return (y1 + y2) / t_x(2);
    }

    t_y t1  = (y2-y0)/(x2-x0);
    t_y t2  = (y3-y1)/(x3-x1);
    t_y a	  = -(x2*(t2+t1)+x1*(-t2-t1)-t_x(2)*y2+ t_x(2)*y1) / det;
    t_y b   = (x1*(x2*(t2-t1)- t_x(3)*y2)+x22*(t2+ t_x(2)*t1)+x12*(t_x(-2)*t2-t1)- t_x(3)*x2*y2+(t_x(3)*x2+3*x1)*y1) / det;
    t_y c   = -(x1*(x22*(t_x(2)*t2+t1)- t_x(6)*x2*y2)-x13*t2+x12*x2*(-t2- t_x(2)*t1)+x23*t1+ t_x(6)*x1*x2*y1) / det;
    t_y d   = (x12*(x22*(t2-t1)- t_x(3)*x2*y2)+x13*(y2-x2*t2)+x1*x23*t1+(t_x(3)*x1*x22-x23)*y1) / det;
    
    return a*x*x*x+b*x*x+c*x+d;
  }
}

#endif
