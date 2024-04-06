/*!*********************************************************************
\file   MenuToQuit.cs
\author loh.j\@digipen.edu
\date   15-March-2024
\brief  
  Script to attach to button to verify quitting.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Button.Pause
{

  public class MenuToQuit : TextButtonBase
  {
    /*!*********************************************************************
    \brief
      Displays the confirmation popup for quitting
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnRelease(uint entity)
    {
      base.OnRelease(entity);
			Utils.PlaySoundF("Quit", (float)m_rng.NextDouble() * (0.6f - 0.75f) + 0.6f, Utils.ChannelType.SFX, false);
			Utils.SetIsActiveEntity(Utils.GetEntity("Confirm Quit"), true);
      
    }

    /*!*********************************************************************
    \brief
      Triggers the hover effect of the button
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public override void OnHoverEnter(uint entity)
    {
      base.OnHoverEnter(entity);
      Utils.PlaySoundF("MenuHoverOverSFX2", 1.0f, Utils.ChannelType.SFX, false);
    }

  }


}
