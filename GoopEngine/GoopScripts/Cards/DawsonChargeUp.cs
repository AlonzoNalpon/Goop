using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts
{
  internal class DawsonChargeUp : CardBase
  {
    public DawsonChargeUp(CardID cardID) : base(cardID)
    {
    }

    public override void Play(ref Stats source, ref Stats target)
    {
      target.TakeDamage(source.DamageDealt(Value));
      target.m_buffs.AddDebuff(new Buff(Buff.BuffType.ADDITIVE, Value, Duration));
    }
  }
}
