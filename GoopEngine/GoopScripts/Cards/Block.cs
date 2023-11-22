using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;

namespace GoopScripts.Cards
{
  internal class Block : CardBase
  {
    public Block(CardID cardID) : base(cardID)
    {
    }

    public override void Play(ref Stats source, ref Stats target)
    {
      // Is technically a positive debuff as it reduces damage dealt by enemy
      //source.m_buffs.AddDebuff(new Buff(Buff.BuffType.SUBTRACTIVE, Value, Duration));
    }
  }
}
