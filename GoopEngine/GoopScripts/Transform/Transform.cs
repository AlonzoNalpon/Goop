using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace GoopScripts
{
  public struct Transform
  {
    public Vec2<double> m_pos  { get; set; }
    public Vec2<double> m_scale { get; set; }
    public double m_rot { get; set; }

    public Transform(Vec2<double> pos, Vec2<double> scale, double rot)
    {
      m_pos = pos;
      m_scale = scale;
      m_rot = rot;
    }
  }
}
