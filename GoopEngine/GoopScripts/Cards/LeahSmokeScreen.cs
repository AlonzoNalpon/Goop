using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts
{
  internal class LeahSmokeScreen : CardBase
  {
    public LeahSmokeScreen(CardID cardID) : base(cardID)
    {
    }

    public override void Play(ref Stats source, ref Stats target)
    {
      target.m_buffs.AddDebuff(new Buff(Buff.BuffType.MULTIPLICATIVE, Value, Duration));
    }
  }
}
