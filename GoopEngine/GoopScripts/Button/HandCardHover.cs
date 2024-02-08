using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class HandCardHover
  {
    static public uint m_cardHover; // holds entity for hover effect
    public void OnHoverEnter(uint entity)
    {
      Vec3<double> pos = Utils.GetWorldPosition(entity);
      pos.Z -= 5.0;
      Utils.SetPosition(m_cardHover, pos);
      Utils.SetIsActiveEntity(m_cardHover, true);
    }

    public void OnHoverExit(uint entity)
    {
      Utils.SetIsActiveEntity(m_cardHover, false);
    }
  }
}
