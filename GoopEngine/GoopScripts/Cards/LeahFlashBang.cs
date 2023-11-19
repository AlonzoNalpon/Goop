using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;

namespace GoopScripts.Cards
{
  internal class LeahFlashBang : CardBase
  {
    public LeahFlashBang(CardID cardID) : base(cardID)
    {
    }

    public override void Play(ref Stats source, ref Stats target)
    {
     // target.m_buffs.AddBuff(new Buff(Buff.BuffType.DIVISIVE, Value, Duration));
    }
  }
}
