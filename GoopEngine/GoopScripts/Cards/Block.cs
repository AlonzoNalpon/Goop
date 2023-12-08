/*!*********************************************************************
\file   Block.cs
\author w.chinkitbryan\@digipen.edu
\date   26-November-2023
\brief  
  Block card, derived from card base.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;

namespace GoopScripts.Cards
{
  internal class Block : CardBase
	{
		/*!*********************************************************************
		\brief
			Overloaded constructor of a card
		\param cardID
			ID of the card 
		************************************************************************/
		public Block(CardID cardID) : base(cardID)
    {
    }

		/*!*********************************************************************
		\brief
      Plays a card with reference from a source and target entity
		\param source
      Reference to a source entity
    \param source
      Reference to a target entity
		************************************************************************/
		public override void Play(ref Stats source, ref Stats target)
    {
      // Is technically a positive debuff as it reduces damage dealt by enemy
      //source.m_buffs.AddDebuff(new Buff(Buff.BuffType.SUBTRACTIVE, Value, Duration));
    }
  }
}
