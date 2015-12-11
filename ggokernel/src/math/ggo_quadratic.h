namespace ggo
{
  template <typename T>
  bool quadratic_solve(T deg2, T deg1, T deg0, T & sol1, T & sol2)
  {
    T d = deg1 * deg1 - 4 * deg2 * deg0;
    if (d < 0)
    {
      return false;
    }
    
    T e = std::sqrt(d);
    T f = -1 / (2 * deg2);
    sol1 = (deg1 - e) * f;
    sol2 = (deg1 + e) * f;
    
    return true;
  }
}
