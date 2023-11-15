using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;
using static GoopScripts.BuffManager;
using System;
using System.Linq;

namespace GoopScripts
{
  internal class Attack : CardBase
  {
    public Attack(CardID cardID) : base(cardID)
    {
    }

    public override void Play(ref Stats source, ref Stats target)
		{
      target.TakeDamage(source.DamageDealt(Value));
    }
	}
}