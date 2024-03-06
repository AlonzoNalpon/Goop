using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;

namespace GoopScripts.Button
{
  public class TextButtonBase : IButtonClick, IButtonRelease, IButtonHoverEnter, IButtonHoverExit
  {
    public enum BUTTON_COLORS
    {
      DEFAULT_TEXT,
      HIGHLIGHT_TEXT,
      DEPRESSED_TEXT
    }

    public Dictionary<BUTTON_COLORS, Vec4<int>> m_buttonClrs;
    Random rng;

    public TextButtonBase() 
    {
      rng = new Random();
      m_buttonClrs = new Dictionary<BUTTON_COLORS, Vec4<int>>();
      m_buttonClrs[BUTTON_COLORS.DEFAULT_TEXT] = new Vec4<int>(228, 212, 198, 255);
      m_buttonClrs[BUTTON_COLORS.HIGHLIGHT_TEXT] = new Vec4<int>();
      m_buttonClrs[BUTTON_COLORS.DEPRESSED_TEXT] = new Vec4<int>();
    }

    public virtual void OnClick(uint entity)
    {
      Utils.UpdateSprite(entity, "Button_Base_Disabled");
    }

    public virtual void OnRelease(uint entity)
    {
      Utils.UpdateSprite(entity, "Button_Base");
      Utils.PlaySoundF("SFX_ButtonClick", (float)rng.NextDouble() * (0.4f - 0.65f), Utils.ChannelType.SFX, false);
    }

    public virtual void OnHoverEnter(uint entity)
    {
    }

    public virtual void OnHoverExit(uint entity)
    {
    }
  }
}
