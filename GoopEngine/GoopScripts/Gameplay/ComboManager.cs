using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;

namespace GoopScripts.Gameplay
{
  public class ComboManager
  {
    public void ResolveCombo(ref Stats source)
    {
      
    }

    public void ComboEffects(CardBase.CardID card1, CardBase.CardID card2, ref int attack, ref int block, ref Queue nextTurn)
    {
      //shield
      if (card1 == CardBase.CardID.SHIELD)
      {
        if (card2 == CardBase.CardID.SHIELD)
        {
          //+1 block
          ++block;
        }

        //basic enemy
        else if (card2 == CardBase.CardID.BASIC_ATTACK)
        {
          //-2 player shield next turn
          nextTurn.Enqueue(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 2.0f, 1));
        }

        //dawson
        else if (card2 == CardBase.CardID.DAWSON_BEAM)
        {
          //remove one random effect on player
        }
        else if (card2 == CardBase.CardID.DAWSON_SWING)
        {
          //+2 block, +1 attack
          block += 2;
          ++attack;
        }

        //player
        else if (card2 == CardBase.CardID.LEAH_BEAM)
        {
          //remove one random effect on enemy
        }
        else if (card2 == CardBase.CardID.LEAH_STRIKE)
        {
          //draw an extra card

        }
      }

      //basic enemy
      if (card1 == CardBase.CardID.BASIC_ATTACK)
      {
        if (card2 == CardBase.CardID.BASIC_ATTACK)
        {
          //+1 damage
          ++attack;
        }
        else if (card2 == CardBase.CardID.SHIELD)
        {
          //-2 player damage next turn
          nextTurn.Enqueue(new Buff(Buff.BuffType.REDUCE_DMG_TAKEN, 2.0f, 1));
        }
      }

      //dawson
      if (card1 == CardBase.CardID.DAWSON_BEAM)
      {
        if (card2 == CardBase.CardID.DAWSON_BEAM)
        {
          //+1 damage
          ++attack;
        }
        else if (card2 == CardBase.CardID.DAWSON_SWING)
        {
          //+2 damage
          attack += 2;
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
          attack += 2;
        }
        else if (card2 == CardBase.CardID.DAWSON_SWING)
        {
          //x150% more damage next round
        }
        else if (card2 == CardBase.CardID.SHIELD)
        {
          //bleed -> -1 player health next round -> basically +1 attack next round
          nextTurn.Enqueue(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1));
        }
      }

      //leah
      if (card1 == CardBase.CardID.LEAH_BEAM)
      {
        if (card2 == CardBase.CardID.LEAH_BEAM)
        {
          //+1 damage
          ++atack;
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
          ++attack;
        }
        else if (card2 == CardBase.CardID.LEAH_STRIKE)
        {
          //penetrate shield
        }
        else if (card2 == CardBase.CardID.SHIELD)
        {
          //-2 enemy shield
          attack += 2;
        }
      }
    }
  }
}
