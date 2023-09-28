using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts
{
   public struct Vec2<T>
  {
    public T x { get; set; }
    public T y { get; set; }

    public Vec2(T xVal, T yVal)
    {
      x = xVal;
      y = yVal;
    }
  }
}
