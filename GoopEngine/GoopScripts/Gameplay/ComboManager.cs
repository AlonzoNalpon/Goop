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
    /*
    Random rng = new Random();
    int chance = rng.Next(1, 2);
    if (chance == 1)
    {
      enemy.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 1));
    }
    */
    public static void ComboPlayer(ref Stats source, ref Stats target)
    {
      //first pair
      if (source.m_deckMngr.m_queue.Length >= 2)
      {
        switch (CardManager.Get(source.m_deckMngr.m_queue[0]).Type)
        {
          case CardBase.CardType.ATTACK:
            switch (CardManager.Get(source.m_deckMngr.m_queue[1]).Type)
            {
              case CardBase.CardType.ATTACK:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1));
                break;

              case CardBase.CardType.BLOCK:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_BLOCK, 1.0f, 1));
                break;

              case CardBase.CardType.SPECIAL:
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1));
                source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 1));
                break;

              default:
              break;
            }
            break;

          case CardBase.CardType.BLOCK:
            switch (CardManager.Get(source.m_deckMngr.m_queue[1]).Type)
            {
              case CardBase.CardType.ATTACK:
                break;

              case CardBase.CardType.BLOCK:
                break;

              case CardBase.CardType.SPECIAL:
                break;

              default:
                break;
            }
            break;

          case CardBase.CardType.SPECIAL:
            switch (CardManager.Get(source.m_deckMngr.m_queue[1]).Type)
            {
              case CardBase.CardType.ATTACK:
                break;

              case CardBase.CardType.BLOCK:
                break;

              case CardBase.CardType.SPECIAL:
                break;

              default:
                break;
            }
            break;

          default:
            break;
        }
        //second pair
        if (source.m_deckMngr.m_queue.Length == 3)
        {
          
        }
      }
    }
  }
}
