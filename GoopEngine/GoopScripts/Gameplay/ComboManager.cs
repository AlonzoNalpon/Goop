using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;

namespace GoopScripts.Gameplay
{
  public class ComboManager
  {
    public static void Combo(ref Stats source, ref Stats target, int firstCardIndex)
    {
      ////Console.WriteLine($"Number of cards in queue: {source.m_deckMngr.m_queue.Length}");
      //Console.WriteLine($"Card types in queue:");
      //foreach (var CardID in source.m_deckMngr.m_queue)
      //{
      //  Console.WriteLine($"{CardManager.Get(CardID).Type.ToString()}");
      //}
      //first pair
      if (source.m_deckMngr.m_queue.Length >= 2)
      {
        int secondCardIndex = (firstCardIndex == 0) ? 1 : 2;
        if (firstCardIndex == 0)
          Console.WriteLine("Resolve first combo for entity: " + source.entityID);
        else
          Console.WriteLine("Resolve second combo for entity: " + source.entityID);

        switch (CardManager.Get(source.m_deckMngr.m_queue[firstCardIndex]).Type)
        {
          case CardBase.CardType.ATTACK:
            switch (CardManager.Get(source.m_deckMngr.m_queue[secondCardIndex]).Type)
            {
              case CardBase.CardType.ATTACK:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 0, "Atk Up"));
                //Console.WriteLine("Combo: Atk & Atk");
                break;

              case CardBase.CardType.BLOCK:
                source.m_deckMngr.Draw();
                //Console.WriteLine("Combo: Atk & Block");
                break;

              case CardBase.CardType.SPECIAL:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 0, "Atk Up"));

                Random rng = new Random();
                int chance = rng.Next(1, 2);
                if (chance == 1)
                {
                  target.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 0, "Blinded"));
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
            switch (CardManager.Get(source.m_deckMngr.m_queue[secondCardIndex]).Type)
            {
              case CardBase.CardType.ATTACK:
                source.m_deckMngr.Draw();
                //Console.WriteLine("Combo: Block & Atk");
                break;

              case CardBase.CardType.BLOCK:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_BLOCK, 1.0f, 0, "Block Up"));
                //Console.WriteLine("Combo: Block & Block");
                break;

              case CardBase.CardType.SPECIAL:
                Random rng = new Random();
                int chance = rng.Next(1, 2);
                if (chance == 1)
                {
                  //Console.WriteLine("Combo enemy is skipped");
                  target.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 0.0f, 0, "Skipped"));
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
            switch (CardManager.Get(source.m_deckMngr.m_queue[secondCardIndex]).Type)
            {
              case CardBase.CardType.ATTACK:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 0, "Atk Up"));

                Random rng1 = new Random();
                int chance1 = rng1.Next(1, 2);
                if (chance1 == 1)
                {
                  target.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 0, "Blinded"));
                }
                //Console.WriteLine("Combo: Special & Atk");
                break;

              case CardBase.CardType.BLOCK:
                Random rng2 = new Random();
                int chance2 = rng2.Next(1, 2);
                if (chance2 == 1)
                {
                  target.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 0, "Skipped"));
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

      }

      else
        Console.WriteLine("Dont need to resolve Combo for entity: " + source.entityID);
    }
  }
}
