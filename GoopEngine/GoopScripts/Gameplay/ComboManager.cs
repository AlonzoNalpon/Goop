using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;

namespace GoopScripts.Gameplay
{
  public class ComboManager
  {
    public void ResolveCombo(CardBase.CardID card1, CardBase.CardID card2)
    {
      //shield
      if (card1 == CardBase.CardID.SHIELD)
      {
        if (card2 == CardBase.CardID.SHIELD)
        {
          //+1 block
        }

        //basic enemy
        else if (card2 == CardBase.CardID.BASIC_ATTACK)
        {
          //-2 player shield
        }

        //dawson
        else if (card2 == CardBase.CardID.DAWSON_BEAM)
        {
          //remove one random effect on player
        }
        else if (card2 == CardBase.CardID.DAWSON_SWING)
        {
          //+2 block, +1 attack
        }

        //player
        else if (card2 == CardBase.CardID.LEAH_BEAM)
        {
          //remove one random effect on enemy
        }
        else if (card2 == CardBase.CardID.LEAH_STRIKE)
        {
          //reflect 50% enemy damage
        }
      }

      //basic enemy
      if (card1 == CardBase.CardID.BASIC_ATTACK)
      {
        if (card2 == CardBase.CardID.BASIC_ATTACK)
        {
          //+1 damage
        }
        else if (card2 == CardBase.CardID.SHIELD)
        {
          //-2 player damage
        }
      }

      //dawson
      if (card1 == CardBase.CardID.DAWSON_BEAM)
      {
        if (card2 == CardBase.CardID.DAWSON_BEAM)
        {
          //+1 damage
        }
        else if (card2 == CardBase.CardID.DAWSON_SWING)
        {
          //+2 damage
        }
        else if (card2 == CardBase.CardID.SHIELD)
        {
          //reflect 50% player damage
        }
      }
      else if (card1 == CardBase.CardID.DAWSON_SWING)
      {
        if (card2 == CardBase.CardID.DAWSON_BEAM)
        {
          //+2 damage
        }
        else if (card2 == CardBase.CardID.DAWSON_SWING)
        {
          //x150% more damage next round
        }
        else if (card2 == CardBase.CardID.SHIELD)
        {
          //+1 damage next round
        }
      }

      //leah
      if (card1 == CardBase.CardID.LEAH_BEAM)
      {
        if (card2 == CardBase.CardID.LEAH_BEAM)
        {
          //+1 damage
        }
        else if (card2 == CardBase.CardID.LEAH_STRIKE)
        {
          //skip enemy's turn
        }
        else if (card2 == CardBase.CardID.SHIELD)
        {
          //reflect 50% enemy damage
        }
      }
      else if (card1 == CardBase.CardID.LEAH_STRIKE)
      {
        if (card2 == CardBase.CardID.LEAH_BEAM)
        {
          //+1 damage & 50% chance of stunning enemy
        }
        else if (card2 == CardBase.CardID.LEAH_STRIKE)
        {
          //penetrate shield
        }
        else if (card2 == CardBase.CardID.SHIELD)
        {
          //-2 enemy shield
        }
      }
    }
  }
}
