/*!*********************************************************************
\file   ChangeToDeckViewer.cs
\author loh.j\@digipen.edu
\date   15 March 2024
\brief  
  Change scene to deck viewer.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Cutscene;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Transition
{
  public class ChangeToDeckViewer : IAnimationEvent
  {
    public void PlayEvent(uint entity)
    {
      Utils.StopChannel(Utils.ChannelType.VOICE);
      Utils.FadeInAudio("CaveWithWaterDrops_Loop", 0.486f, 1.0f);
      Utils.FadeInAudio("Fog", 0.753f, 1.0f);
      Utils.TransitionToScene("HomeBase");
    }
  }
}
