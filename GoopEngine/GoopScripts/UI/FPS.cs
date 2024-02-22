/*!*********************************************************************
\file   FPS.cs
\author a.nalpon\@digipen.edu
\date   09 Febuary 2024
\brief
  Script for updating an FPS counter text object

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.UI
{
    public class FPS : Entity
    {

        public int FPS_COUNTER;

        public FPS() { }

        public void OnCreate()
        {
        }

        public void OnUpdate(double deltaTime)
        {
            {
                // SET FPS TEXT TO FPS
                double fps = Utils.GetFPS();
                string text = string.Format("{0:N2}", fps);
                Utils.SetTextComponent(FPS_COUNTER, text);
            }
        }
    }
}
