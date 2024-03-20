/*!*********************************************************************
\file   RewardButton.cs
\author loh.j\@digipen.edu
\date   15 March 2024
\brief  
  Script to attach to button to reward button.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Button
{
  public class RewardButton : TextButtonBase
  {

    public override void OnRelease(uint entity)
    {
      m_clicked = false;
      Utils.PlaySoundF("SFX_ButtonClick", (float)m_rng.NextDouble() * (0.6f - 0.75f) + 0.6f, Utils.ChannelType.SFX, false);
      ((RewardManager)Utils.GetScript("RewardManager", "RewardManager")).AddCardsToDeck();
      if (!m_hovering)
      {
        Utils.UpdateSprite(entity, "Button_Base");
        Vec4<int> clr = m_buttonClrs[BUTTON_COLORS.DEFAULT_TEXT];
        Utils.SetTextColor(Utils.GetChildEntity(entity, "Text"), clr.X, clr.Y, clr.Z, clr.W);
      }
      else
      {
        OnHoverEnter(entity);
      }
    }

  }
}
