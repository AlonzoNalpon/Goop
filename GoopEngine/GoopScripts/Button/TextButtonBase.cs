﻿using System;
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
    bool hovering;
    bool clicked;

    public TextButtonBase() 
    {
      rng = new Random();
      m_buttonClrs = new Dictionary<BUTTON_COLORS, Vec4<int>>();
      m_buttonClrs[BUTTON_COLORS.DEFAULT_TEXT] = new Vec4<int>(228, 212, 198, 255);
      m_buttonClrs[BUTTON_COLORS.HIGHLIGHT_TEXT] = new Vec4<int>(255, 255, 255, 255);
      m_buttonClrs[BUTTON_COLORS.DEPRESSED_TEXT] = new Vec4<int>(114, 106, 99, 255);
    }

    public virtual void OnClick(uint entity)
    {
      clicked = true;
      Utils.UpdateSprite(entity, "Button_Base_Disabled");
      Vec4<int> clr = m_buttonClrs[BUTTON_COLORS.DEPRESSED_TEXT];
      Utils.SetTextColor(Utils.GetChildEntity(entity, "Text"), clr.X, clr.Y, clr.Z, clr.W);
    }

    public virtual void OnRelease(uint entity)
    {
      clicked = false;
      Utils.PlaySoundF("SFX_ButtonClick", (float)rng.NextDouble() * (0.6f - 0.75f) + 0.6f, Utils.ChannelType.SFX, false);
      if (!hovering)
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

    public virtual void OnHoverEnter(uint entity)
    {
      hovering = true;
      Utils.UpdateSprite(entity, "Button_Base_Hover");
      Vec4<int> clr = m_buttonClrs[BUTTON_COLORS.HIGHLIGHT_TEXT];
      Utils.SetTextColor(Utils.GetChildEntity(entity, "Text"), clr.X, clr.Y, clr.Z, clr.W);
    }

    public virtual void OnHoverExit(uint entity)
    {
      hovering = false;
      if (!clicked)
      {
        Utils.UpdateSprite(entity, "Button_Base");
        Vec4<int> clr = m_buttonClrs[BUTTON_COLORS.DEFAULT_TEXT];
        Utils.SetTextColor(Utils.GetChildEntity(entity, "Text"), clr.X, clr.Y, clr.Z, clr.W);
      }
    }
  }
}
