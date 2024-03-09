using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  internal class HTP : Entity
  {
    static public int m_page;

    public void OnCreate()
    {
      m_page = 1;
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP.m_page}"), true);
    }

    public void OnUpdate(double deltaTime)
    {
      if (m_page == 1)
      {
        Utils.SetIsActiveEntity(Utils.GetEntity("HTP_Arrow_Left"), false);
      }
      else if (m_page == 10)
      {
        Utils.SetIsActiveEntity(Utils.GetEntity("HTP_Arrow_Right"), false);
      }
      else
      {
        Utils.SetIsActiveEntity(Utils.GetEntity("HTP_Arrow_Left"), true);
        Utils.SetIsActiveEntity(Utils.GetEntity("HTP_Arrow_Right"), true);
      }
    }
  }
}
