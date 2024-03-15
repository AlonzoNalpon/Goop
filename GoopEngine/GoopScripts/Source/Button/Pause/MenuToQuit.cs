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
    public int PauseMenuID;

		public override void OnClick(uint entity)
    {
      m_clicked = true;
      Utils.UpdateSprite(entity, "Button_Base_Disabled");
      Vec4<int> clr = m_buttonClrs[BUTTON_COLORS.DEPRESSED_TEXT];
      Utils.SetTextColor(Utils.GetChildEntity(entity, "Text"), clr.X, clr.Y, clr.Z, clr.W);
      Utils.SetIsActiveEntity((uint)PauseMenuID, true);
    }
  }
}
