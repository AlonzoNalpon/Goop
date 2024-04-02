/*!*********************************************************************
\file   HTP.cs
\author c.phua\@digipen.edu
\date   15-March-2024
\brief  Script for how to play screen with arrows to control the page.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  internal class HTP_Popup : Entity
  {
    static public int m_page;

    public void OnCreate()
    {
      m_page = 1;
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{m_page}"), true);
    }

    public void OnUpdate(double deltaTime)
    {
      if (Utils.GetIsActiveEntity(Utils.GetEntity("[Menu] How To Play")))
      {
        if (m_page == 1)
        {
          Utils.SetIsActiveEntity(Utils.GetEntity("HTP_Arrow_Left"), false);
        }
        else if (m_page == 11)
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
}
