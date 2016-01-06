#ifndef __GGO_INTERPOLATE_1D__
#define __GGO_INTERPOLATE_1D__

namespace ggo
{
  template <typename T_X, typename T_Y>
  T_Y linear_interpolation(T_X x0, T_Y y0, T_X x1, T_Y y1, T_X x)
  {
    T_X det = x0 - x1;
    if (std::abs(det) < T_X(0.0000001))
    {
      return (y0 + y1) / T_X(2);
    }
    
    T_Y a = y0 - y1;
    T_Y b = x0 * y1 - x1 * y0;

    return (a * x + b) / det;
  }

  template <typename T_X, typename T_Y>
  T_Y cubic_interpolation(T_X x0, T_Y y0, T_X x1, T_Y y1, T_X x2, T_Y y2, T_X x3, T_Y y3, T_X x)
  {
    if (std::abs(x2 - x0) <= T_X(0.0000001))
    {
      return (y1 + y2) / T_X(2);
    }
    
    if (std::abs(x3 - x1) <= T_X(0.0000001))
    {
      return (y1 + y2) / T_X(2);
    }

    T_X x12 = x1 * x1;
    T_X x13 = x1 * x12;
    T_X x22 = x2 * x2;
    T_X x23 = x2 * x22;
    
    T_X det = 3 * (x1 * x22 - x12 * x2) + x13 - x23;
    if (std::abs(det) <= T_X(0.0000001))
    {
      return (y1 + y2) / T_X(2);
    }

    T_Y t1  = (y2-y0)/(x2-x0);
    T_Y t2  = (y3-y1)/(x3-x1);
    T_Y a	  = -(x2*(t2+t1)+x1*(-t2-t1)-T_X(2)*y2+ T_X(2)*y1) / det;
    T_Y b   = (x1*(x2*(t2-t1)- T_X(3)*y2)+x22*(t2+ T_X(2)*t1)+x12*(T_X(-2)*t2-t1)- T_X(3)*x2*y2+(T_X(3)*x2+3*x1)*y1) / det;
    T_Y c   = -(x1*(x22*(T_X(2)*t2+t1)- T_X(6)*x2*y2)-x13*t2+x12*x2*(-t2- T_X(2)*t1)+x23*t1+ T_X(6)*x1*x2*y1) / det;
    T_Y d   = (x12*(x22*(t2-t1)- T_X(3)*x2*y2)+x13*(y2-x2*t2)+x1*x23*t1+(T_X(3)*x1*x22-x23)*y1) / det;
    
    return a*x*x*x+b*x*x+c*x+d;
  }
}

#endif
