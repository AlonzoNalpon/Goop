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
      if (player.m_deckMngr.m_queue.Length >= 2)
      {
        switch (player.m_deckMngr.m_queue[0])
        {
          case CardBase.CardID.LEAH_SHIELD:
            switch (player.m_deckMngr.m_queue[1])
            {
              case CardBase.CardID.LEAH_SHIELD:
                //+1 block
                player.AddBlock(1);
                break;

              case CardBase.CardID.LEAH_BEAM:
                //remove one effect from enemy
                if (enemy.m_buffs.Buffs.Count > 0)
                  enemy.m_buffs.RemoveBuff();
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
                player.AddAttack(reflect);
                break;

              case CardBase.CardID.LEAH_BEAM:
                //+1 damage
                player.AddAttack(1);
                break;

              case CardBase.CardID.LEAH_STRIKE:
                //skip enemy's turn
                enemy.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1));
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
                enemy.AddBlock(-2);
                break;

              case CardBase.CardID.LEAH_BEAM:
                //+1 damage & 50% chance to stun enemy
                player.AddAttack(1);

                Random rng = new Random();
                int chance = rng.Next(1, 2);
                if (chance == 1)
                {
                  enemy.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1));
                }
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

        if (player.m_deckMngr.m_queue.Length == 3)
        {
          switch (player.m_deckMngr.m_queue[1])
          {
            case CardBase.CardID.LEAH_SHIELD:
              switch (player.m_deckMngr.m_queue[2])
              {
                case CardBase.CardID.LEAH_SHIELD:
                  //+1 block
                  player.AddBlock(1);
                  break;

                case CardBase.CardID.LEAH_BEAM:
                  //remove one effect from enemy
                  if (enemy.m_buffs.Buffs.Count > 0)
                    enemy.m_buffs.RemoveBuff();
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
              switch (player.m_deckMngr.m_queue[2])
              {
                case CardBase.CardID.LEAH_SHIELD:
                  //reflect 50% enemy damage
                  int reflect = enemy.m_attack / 2;
                  player.AddAttack(reflect);
                  break;

                case CardBase.CardID.LEAH_BEAM:
                  //+1 damage
                  player.AddAttack(1);
                  break;

                case CardBase.CardID.LEAH_STRIKE:
                  //skip enemy's turn
                  enemy.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1));
                  break;

                default:
                  break;
              }
              break;

            case CardBase.CardID.LEAH_STRIKE:
              switch (player.m_deckMngr.m_queue[2])
              {
                case CardBase.CardID.LEAH_SHIELD:
                  //-2 enemy shield
                  enemy.AddBlock(-2);
                  break;

                case CardBase.CardID.LEAH_BEAM:
                  //+1 damage & 50% chance to stun enemy
                  player.AddAttack(1);

                  Random rng = new Random();
                  int chance = rng.Next(1, 2);
                  if (chance == 1)
                  {
                    enemy.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1));
                  }
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
      }
    }

    public static void ComboEnemy(ref Stats player, ref Stats enemy)
    {
      if (enemy.m_type == CharacterType.BASIC_ENEMY)
      {
        if (enemy.m_deckMngr.m_queue.Length >= 2)
        {
          switch (enemy.m_deckMngr.m_queue[0])
          {
            case CardBase.CardID.BASIC_ATTACK:
              switch (enemy.m_deckMngr.m_queue[1])
              {
                case CardBase.CardID.BASIC_ATTACK:
                  //+1 attack
                  enemy.AddAttack(1);
                  break;

                case CardBase.CardID.BASIC_SHIELD:
                  //-2 player damage next turn
                  player.m_buffs.AddBuff(new Buff(Buff.BuffType.REDUCE_ATK_DEALT, 2.0f, 1));
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
                  player.m_buffs.AddBuff(new Buff(Buff.BuffType.REDUCE_SHIELD, 1.0f, 1));
                  break;

                case CardBase.CardID.BASIC_SHIELD:
                  //+1 block
                  enemy.AddBlock(1);
                  break;

                default:
                  break;
              }
              break;

            default:
              break;
          }

          if (enemy.m_deckMngr.m_queue.Length == 3)
          {
            switch (enemy.m_deckMngr.m_queue[1])
            {
              case CardBase.CardID.BASIC_ATTACK:
                switch (enemy.m_deckMngr.m_queue[2])
                {
                  case CardBase.CardID.BASIC_ATTACK:
                    //+1 attack
                    enemy.AddAttack(1);
                    break;

                  case CardBase.CardID.BASIC_SHIELD:
                    //-2 player damage next turn
                    player.m_buffs.AddBuff(new Buff(Buff.BuffType.REDUCE_ATK_DEALT, 2.0f, 1));
                    break;

                  default:
                    break;
                }
                break;

              case CardBase.CardID.BASIC_SHIELD:
                switch (enemy.m_deckMngr.m_queue[2])
                {
                  case CardBase.CardID.BASIC_ATTACK:
                    //-1 player shield next turn
                    player.m_buffs.AddBuff(new Buff(Buff.BuffType.REDUCE_SHIELD, 1.0f, 1));
                    break;

                  case CardBase.CardID.BASIC_SHIELD:
                    //+1 block
                    enemy.AddBlock(1);
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

      else if (enemy.m_type == CharacterType.BOSS_P1 || enemy.m_type == CharacterType.BOSS_P2)
      {
        if (enemy.m_deckMngr.m_queue.Length >= 2)
        {
          switch (enemy.m_deckMngr.m_queue[0])
          {
            case CardBase.CardID.DAWSON_BEAM:
              switch (enemy.m_deckMngr.m_queue[1])
              {
                case CardBase.CardID.DAWSON_BEAM:
                  //+1 attack
                  enemy.AddAttack(1);
                  break;

                case CardBase.CardID.DAWSON_SWING:
                  //+2 attack
                  enemy.AddAttack(2);
                  break;

                case CardBase.CardID.DAWSON_SHIELD:
                  //reflect 50% player damage
                  int reflect = player.m_attack / 2;
                  enemy.AddAttack(reflect);
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
                  enemy.AddAttack(2);
                  break;

                case CardBase.CardID.DAWSON_SWING:
                  //x150% damage next turn
                  enemy.m_buffs.AddBuff(new Buff(Buff.BuffType.MULTIPLY_ATK_DEALT, 1.5f, 1));
                  break;

                case CardBase.CardID.DAWSON_SHIELD:
                  //bleed -> -1 player health next turn -> basically +1 attack but yes.
                  player.m_buffs.AddBuff(new Buff(Buff.BuffType.BLEED, 1.0f, 1));
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
                  if (player.m_buffs.Buffs.Count > 0)
                    player.m_buffs.RemoveBuff();
                  break;

                case CardBase.CardID.DAWSON_SWING:
                  //+2 block, +1 attack
                  enemy.AddBlock(2);
                  enemy.AddAttack(1);
                  break;

                case CardBase.CardID.DAWSON_SHIELD:
                  //+1 block
                  enemy.AddBlock(1);
                  break;

                default:
                  break;
              }
              break;

            default:
              break;
          }

          if (enemy.m_deckMngr.m_queue.Length == 3)
          {
            switch (enemy.m_deckMngr.m_queue[1])
            {
              case CardBase.CardID.DAWSON_BEAM:
                switch (enemy.m_deckMngr.m_queue[2])
                {
                  case CardBase.CardID.DAWSON_BEAM:
                    //+1 attack
                    enemy.AddAttack(1);
                    break;

                  case CardBase.CardID.DAWSON_SWING:
                    //+2 attack
                    enemy.AddAttack(2);
                    break;

                  case CardBase.CardID.DAWSON_SHIELD:
                    //reflect 50% player damage
                    int reflect = player.m_attack / 2;
                    enemy.AddAttack(reflect);
                    break;

                  default:
                    break;
                }
                break;

              case CardBase.CardID.DAWSON_SWING:
                switch (enemy.m_deckMngr.m_queue[2])
                {
                  case CardBase.CardID.DAWSON_BEAM:
                    //+2 attack
                    enemy.AddAttack(2);
                    break;

                  case CardBase.CardID.DAWSON_SWING:
                    //x150% damage next turn
                    enemy.m_buffs.AddBuff(new Buff(Buff.BuffType.MULTIPLY_ATK_DEALT, 1.5f, 1));
                    break;

                  case CardBase.CardID.DAWSON_SHIELD:
                    //bleed -> -1 player health next turn -> basically +1 attack but yes.
                    player.m_buffs.AddBuff(new Buff(Buff.BuffType.BLEED, 1.0f, 1));
                    break;

                  default:
                    break;
                }
                break;

              case CardBase.CardID.DAWSON_SHIELD:
                switch (enemy.m_deckMngr.m_queue[2])
                {
                  case CardBase.CardID.DAWSON_BEAM:
                    //remove one player effect
                    if (player.m_buffs.Buffs.Count > 0)
                      player.m_buffs.RemoveBuff();
                    break;

                  case CardBase.CardID.DAWSON_SWING:
                    //+2 block, +1 attack
                    enemy.AddBlock(2);
                    enemy.AddAttack(1);
                    break;

                  case CardBase.CardID.DAWSON_SHIELD:
                    //+1 block
                    enemy.AddBlock(1);
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
    }
  }
}
