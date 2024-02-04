/*!*********************************************************************
\file   BlankCard.cs
\author w.chinkitbryan\@digipen.edu
\date   26-November-2023
\brief  
  Blank card, derived from card base. This class serves the purpose
 of doing nothing as a default case
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Gameplay;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Cards
{
  internal class BlankCard : CardBase
	{
		/*!*********************************************************************
		\brief
			Overloaded constructor of a card
		\param cardID
			ID of the card 
		************************************************************************/
		public BlankCard(CardID cardID, CardType cardType, string animSprite) : base(cardID, cardType, animSprite)
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
      // Does nothing
    }
  }
}
