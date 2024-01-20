/*!*********************************************************************
\file   CardBase.cs
\author w.chinkitbryan\@digipen.edu
\date   26-November-2023
\brief  
Blank card, derived from card base. This class serves the purpose
of doing nothing as a default case
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using GoopScripts.Gameplay;

namespace GoopScripts.Cards
{
  public abstract class CardBase : MonoBehaviour
  {
    public enum CardID
    {
      NO_CARD,

      // Basic enemy cards
      BASIC_ATTACK,
      BASIC_SHIELD,
      SPECIAL_SCREECH,

      // Player cards
      LEAH_BEAM,
      LEAH_STRIKE,
      LEAH_SHIELD,

      SPECIAL_FLASHBANG,
      SPECIAL_SMOKESCREEN,
      SPECIAL_RAGE,

      // Dawson enemy cards
      DAWSON_BEAM,
      DAWSON_SWING,
      DAWSON_SHIELD,

      SPECIAL_CHARGEUP,
      SPECIAL_TIMEWRAP,

      TOTAL_CARDS,
    }

    CardID cardID;
    float m_actionVal;
    int m_duration;

    public float Value { get { return m_actionVal; } set { m_actionVal = value; } }
    public int Duration { get { return m_duration; } set { m_duration = value; } }

    /*!*********************************************************************
    \brief
      Overloaded constructor of a card
    \param cardID
      ID of the card 
    ************************************************************************/
    public CardBase(CardID cardID) { this.cardID = cardID; }

    /*!*********************************************************************
    \brief
      Does nothing. This functions must be defined by derived classes
      to excute a behaviour of the card
    \param source
      Reference to a source entity
    \param source
      Reference to a target entity
    ************************************************************************/
    public abstract void Play(ref Stats source, ref Stats target);
  }
}
