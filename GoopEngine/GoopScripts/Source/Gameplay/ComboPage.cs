using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.Source.Gameplay
{
    internal class ComboPage : Entity
    {
        public void OnCreate()
        {
        }

        public void OnUpdate(double deltaTime)
        {

            if (Utils.IsKeyHeld(Input.KeyCode.TAB))
            {
                Utils.SetIsActiveEntity(Utils.GetEntity("ComboList"), true);
            }

            if (Utils.IsKeyReleased(Input.KeyCode.TAB))
            {
                Utils.SetIsActiveEntity(Utils.GetEntity("ComboList"), false);
            }
        }
    }
}
