/*!*********************************************************************
\file   RewardCard.cs
\author loh.j\@digipen.edu
\date   15-March-2024
\brief  
  Script for generating randomed reward cards.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
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
    uint box;
    bool selected = false;
    Random rng = new Random();


    public RewardCard() { }

    /*!*********************************************************************
    \brief
      Creates a highlight border around the card when it is selected, and
      unselects it if it was already selected.
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public virtual void OnClick(uint entity)
    {
      if (selected)
      {
        Vec3<double> pos = Utils.GetWorldPosition(entity);
        Utils.SetPosition(m_cardHover, pos);
        Utils.SetIsActiveEntity(m_cardHover, true);
        Utils.DestroyEntity(box);
        RewardManager.CardUnselected(entity);
        Utils.PlaySoundF("SFX_ButtonClick", (float)rng.NextDouble() * (0.6f - 0.75f) + 0.6f, Utils.ChannelType.SFX, false);
      }

      if (RewardManager.IsFull())
      {
        return;
      }

      if (!selected)
      {
        box = Utils.CreateObject("Box", new Vec3<double>(0, 0, 1), new Vec3<double>(), new Vec3<double>(), entity);
        Utils.UpdateSprite(box, "CardSelected");
        Utils.SetScale(box, new Vec3<double>(1, 1, 1));
        Utils.SetIsActiveEntity(m_cardHover, false);
        RewardManager.CardSelected(entity);
        Utils.PlaySoundF("SFX_ButtonClick", (float)rng.NextDouble() * (0.6f - 0.75f) + 0.6f, Utils.ChannelType.SFX, false);
      }
      selected = !selected;
    }

    /*!*********************************************************************
    \brief
      Displays the hover effect when the card is hovered upon
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public virtual void OnHoverEnter(uint entity)
    {
      if (RewardManager.IsFull())
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

    /*!*********************************************************************
    \brief
      Removes the hover effect when the card is no longer hovered on
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public virtual void OnHoverExit(uint entity)
    {
      if (RewardManager.IsFull())
      {
        return;
      }

      if (!selected)
      {
        Utils.SetIsActiveEntity(m_cardHover, false);
      }
    }
  }
}
