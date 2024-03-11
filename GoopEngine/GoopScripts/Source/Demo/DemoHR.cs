using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Demo
{
    internal class DemoHR : IAnimationEvent
    {
        public void PlayEvent(uint entity)
        {
            Utils.PlaySoundF("SFX_BodyFall3", 1.0f, Utils.ChannelType.SFX, false);
            Utils.PlayAnimation("SS_Leah_DeathNoShield", entity);
            Utils.SetTextComponent((int)entity, "IT WORKED");
        }
    }
}
