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
    public static void ComboPlayer(ref Stats player, ref Stats enemy)
    {
      switch (player.m_deckMngr.m_queue[0])
      {
        case CardBase.CardID.LEAH_SHIELD:
          switch (player.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.LEAH_SHIELD:
              //+1 block
              ++player.m_block;
              break;

            case CardBase.CardID.LEAH_BEAM:
              //remove one effect from enemy
              break;

            case CardBase.CardID.LEAH_STRIKE:
              //draw card
              player.m_deckMngr.Draw();
              break;

            default:
              break;
          }
          break;

        case CardBase.CardID.LEAH_BEAM:
          switch (player.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.LEAH_SHIELD:
              //reflect 50% enemy damage
              int reflect = enemy.m_attack / 2;
              player.m_attack += reflect;
              break;

            case CardBase.CardID.LEAH_BEAM:
              //+1 damage
              ++player.m_attack;
              break;

            case CardBase.CardID.LEAH_STRIKE:
              //skip enemy's turn
              break;

            default:
              break;
          }
          break;

        case CardBase.CardID.LEAH_STRIKE:
          switch (player.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.LEAH_SHIELD:
              //-2 enemy shield
              enemy.m_block -= 2;
              break;

            case CardBase.CardID.LEAH_BEAM:
              //+1 damage & 50% chance to stun enemy
              ++player.m_attack;
              break;

            case CardBase.CardID.LEAH_STRIKE:
              //penetrate shield
              enemy.m_block = 0;
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
    }

    public static void ComboEnemy(ref Stats player, ref Stats enemy)
    {
      switch (enemy.m_deckMngr.m_queue[0])
      {
        //basic enemy
        case CardBase.CardID.BASIC_ATTACK:
          switch (enemy.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.BASIC_ATTACK:
              //+1 attack
              ++enemy.m_attack;
              break;

            case CardBase.CardID.BASIC_SHIELD:
              //-2 player damage next turn
              break;

            default:
              break;
          }
          break;

        case CardBase.CardID.BASIC_SHIELD:
          switch (enemy.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.BASIC_ATTACK:
              //-1 player shield next turn
              break;

            case CardBase.CardID.BASIC_SHIELD:
              //+1 block
              ++enemy.m_block;
              break;

            default:
              break;
          }
          break;

        //Dawson
        case CardBase.CardID.DAWSON_BEAM:
          switch (enemy.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.DAWSON_BEAM:
              //+1 attack
              ++enemy.m_attack;
              break;

            case CardBase.CardID.DAWSON_SWING:
              //+2 attack
              enemy.m_attack += 2;
              break;

            case CardBase.CardID.DAWSON_SHIELD:
              //reflect 50% player damage
              int reflect = player.m_attack / 2;
              enemy.m_attack += reflect;
              break;

            default:
              break;
          }
          break;

        case CardBase.CardID.DAWSON_SWING:
          switch (enemy.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.DAWSON_BEAM:
              //+2 attack
              enemy.m_attack += 2;
              break;

            case CardBase.CardID.DAWSON_SWING:
              //x150% damage next turn
              enemy.m_nextTurn.Enqueue(new Buff(Buff.BuffType.MULTIPLY_ATK_DEALT, 1.5f, 1));
              break;

            case CardBase.CardID.DAWSON_SHIELD:
              //bleed -> -1 player health next turn -> basically +1 attack but yes.
              break;

            default:
              break;
          }
          break;

        case CardBase.CardID.DAWSON_SHIELD:
          switch (enemy.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.DAWSON_BEAM:
              //remove one player effect
              break;

            case CardBase.CardID.DAWSON_SWING:
              //+2 block, +1 attack
              enemy.m_block += 2;
              ++enemy.m_attack;
              break;

            case CardBase.CardID.DAWSON_SHIELD:
              //+1 block
              ++enemy.m_block;
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
    }
  }
}
