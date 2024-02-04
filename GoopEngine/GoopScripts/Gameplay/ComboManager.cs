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
    public static void Combo(ref Stats source, ref Stats target)
    {
      //Console.WriteLine($"Number of cards in queue: {source.m_deckMngr.m_queue.Length}");
      //Console.WriteLine($"Card types in queue:");
      //foreach (var CardID in source.m_deckMngr.m_queue)
      //{
      //  Console.WriteLine($"{CardManager.Get(CardID).Type.ToString()}");
      //}
      //first pair
      if (source.m_deckMngr.m_queue.Length >= 2)
      {
        switch (CardManager.Get(source.m_deckMngr.m_queue[0].Item1).Type)
        {
          case CardBase.CardType.ATTACK:
            switch (CardManager.Get(source.m_deckMngr.m_queue[1].Item1).Type)
            {
              case CardBase.CardType.ATTACK:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1, "Atk Up"));
                //Console.WriteLine("Combo: Atk & Atk");
                break;

              case CardBase.CardType.BLOCK:
                source.m_deckMngr.Draw();
                //Console.WriteLine("Combo: Atk & Block");
                break;

              case CardBase.CardType.SPECIAL:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1, "Atk Up"));
                
                Random rng = new Random();
                int chance = rng.Next(1, 2);
                if (chance == 1)
                {
                  target.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 1, "Blinded"));
                }
                //Console.WriteLine("Combo: Atk & Special");
                break;

              case CardBase.CardType.BLANK_CARD:
                break;

              default:
              break;
            }
            break;

          case CardBase.CardType.BLOCK:
            switch (CardManager.Get(source.m_deckMngr.m_queue[1].Item1).Type)
            {
              case CardBase.CardType.ATTACK:
                source.m_deckMngr.Draw();
                //Console.WriteLine("Combo: Block & Atk");
                break;

              case CardBase.CardType.BLOCK:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_BLOCK, 1.0f, 1, "Block Up"));
                //Console.WriteLine("Combo: Block & Block");
                break;

              case CardBase.CardType.SPECIAL:
                Random rng = new Random();
                int chance = rng.Next(1, 2);
                if (chance == 1)
                {
                  target.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1, "Skipped"));
                }
                //Console.WriteLine("Combo: Block & Special");
                break;

              case CardBase.CardType.BLANK_CARD:
                break;

              default:
                break;
            }
            break;

          case CardBase.CardType.SPECIAL:
            switch (CardManager.Get(source.m_deckMngr.m_queue[1].Item1).Type)
            {
              case CardBase.CardType.ATTACK:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1, "Atk Up"));

                Random rng1 = new Random();
                int chance1 = rng1.Next(1, 2);
                if (chance1 == 1)
                {
                  target.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 1, "Blinded"));
                }
                //Console.WriteLine("Combo: Special & Atk");
                break;

              case CardBase.CardType.BLOCK:
                Random rng2 = new Random();
                int chance2 = rng2.Next(1, 2);
                if (chance2 == 1)
                {
                  target.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1, "Skipped"));
                }
                //Console.WriteLine("Combo: Special & Block");
                break;

              case CardBase.CardType.BLANK_CARD:
                break;

              default:
                break;
            }
            break;

          case CardBase.CardType.BLANK_CARD:
            break;

          default:
            break;
        }

        //second pair
        if (source.m_deckMngr.m_queue.Length == 3)
        {
          switch (CardManager.Get(source.m_deckMngr.m_queue[1].Item1).Type)
          {
            case CardBase.CardType.ATTACK:
              switch (CardManager.Get(source.m_deckMngr.m_queue[1].Item1).Type)
              {
                case CardBase.CardType.ATTACK:
                  source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1, "Atk Up"));
                  //Console.WriteLine("Combo: Atk & Atk");
                  break;

                case CardBase.CardType.BLOCK:
                  source.m_deckMngr.Draw();
                  //Console.WriteLine("Combo: Atk & Block");
                  break;

                case CardBase.CardType.SPECIAL:
                  source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1, "Atk Up"));

                  Random rng = new Random();
                  int chance = rng.Next(1, 2);
                  if (chance == 1)
                  {
                    target.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 1, "Blinded"));
                  }
                  //Console.WriteLine("Combo: Atk & Special");
                  break;

                case CardBase.CardType.BLANK_CARD:
                  break;

                default:
                  break;
              }
              break;

            case CardBase.CardType.BLOCK:
              switch (CardManager.Get(source.m_deckMngr.m_queue[1].Item1).Type)
              {
                case CardBase.CardType.ATTACK:
                  source.m_deckMngr.Draw();
                  //Console.WriteLine("Combo: Block & Atk");
                  break;

                case CardBase.CardType.BLOCK:
                  source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_BLOCK, 1.0f, 1, "Block Up"));
                  //Console.WriteLine("Combo: Block & Block");
                  break;

                case CardBase.CardType.SPECIAL:
                  Random rng = new Random();
                  int chance = rng.Next(1, 2);
                  if (chance == 1)
                  {
                    target.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1, "Skipped"));
                  }
                  //Console.WriteLine("Combo: Block & Special");
                  break;

                case CardBase.CardType.BLANK_CARD:
                  break;

                default:
                  break;
              }
              break;

            case CardBase.CardType.SPECIAL:
              switch (CardManager.Get(source.m_deckMngr.m_queue[1].Item1).Type)
              {
                case CardBase.CardType.ATTACK:
                  source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1, "Atk Up"));

                  Random rng1 = new Random();
                  int chance1 = rng1.Next(1, 2);
                  if (chance1 == 1)
                  {
                    target.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 1, "Blinded"));
                  }
                  //Console.WriteLine("Combo: Special & Atk");
                  break;

                case CardBase.CardType.BLOCK:
                  Random rng2 = new Random();
                  int chance2 = rng2.Next(1, 2);
                  if (chance2 == 1)
                  {
                    target.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1, "Skipped"));
                  }
                  //Console.WriteLine("Combo: Special & Block");
                  break;

                case CardBase.CardType.BLANK_CARD:
                  break;

                default:
                  break;
              }
              break;

            case CardBase.CardType.BLANK_CARD:
              break;

            default:
              break;
          }
        }
      }
    }
  }
}
