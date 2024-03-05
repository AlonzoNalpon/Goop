using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  internal class QuitGame : IButtonClick, IButtonRelease
  {
    public void OnClick(uint entity)
    {
      Utils.PlaySoundF("SFX_ButtonClick", 1.0f, Utils.ChannelType.SFX, false);
      Utils.UpdateSprite(entity, "Button_Base_Disabled");
    }

    public void OnRelease(uint entity)
    {
      Utils.UpdateSprite(entity, "Button_Base");
      Utils.DispatchQuitEvent();
    }
  }
}
