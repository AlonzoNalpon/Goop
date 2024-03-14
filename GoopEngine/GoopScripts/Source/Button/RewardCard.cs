﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;
using GoopScripts.Gameplay;
using GoopScripts.Mono;

namespace GoopScripts.Button
{
  public class RewardCard : IButtonClick, IButtonHoverEnter, IButtonHoverExit
  {
    static public uint m_cardHover;
    static public CardBase.CardID m_type = CardBase.CardID.NO_CARD;
    static public uint m_entityID;
    uint box;
    bool selected = false;


    public RewardCard() { }

    public virtual void OnClick(uint entity)
    {
      if (selected)
      {
        Vec3<double> pos = Utils.GetWorldPosition(entity);
        Utils.SetPosition(m_cardHover, pos);
        Utils.SetIsActiveEntity(m_cardHover, true);
        Utils.DestroyEntity(box);
        RewardManager.m_selectedCards.Remove(m_type);
      }

      if (RewardManager.m_selectedCards.Count() >= 3)
      {
        return;
      }

      if (!selected)
      {
        box = Utils.CreateObject("Box", new Vec3<double>(0, 0, 1), new Vec3<double>(), new Vec3<double>(), entity);
        Utils.UpdateSprite(box, "CardSelected");
        Utils.SetScale(box, new Vec3<double>(1, 1, 1));
        Utils.SetIsActiveEntity(m_cardHover, false);
        RewardManager.m_selectedCards.Add(m_type);
      }

      selected = !selected;
    }

    public virtual void OnHoverEnter(uint entity)
    {
      if (RewardManager.m_selectedCards.Count() >= 3)
      {
        return;
      }

      if (!selected)
      {
        Vec3<double> pos = Utils.GetWorldPosition(entity);
        // pos.Z -= 5.0;
        Utils.SetPosition(m_cardHover, pos);
        Utils.SetIsActiveEntity(m_cardHover, true);
      }
    }

    public virtual void OnHoverExit(uint entity)
    {
      if (RewardManager.m_selectedCards.Count() >= 3)
      {
        return;
      }

      if (!selected)
      {
        Utils.SetIsActiveEntity(m_cardHover, false);
      }
    }
    static public void Deselect()
    {
      Vec3<double> pos = Utils.GetWorldPosition(m_entityID);
      Utils.SetPosition(m_cardHover, pos);
      Utils.SetIsActiveEntity(m_cardHover, true);
      // Utils.DestroyEntity(Utils.GetScriptFromID(m_entityID, "RewardCard").box) ;
      RewardManager.m_selectedCards.Remove(m_type);
    }
  }
}
