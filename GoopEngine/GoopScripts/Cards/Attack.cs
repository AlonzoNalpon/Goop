using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;
using static GoopScripts.Gameplay.BuffManager;
using System;
using System.Linq;
using GoopScripts.Gameplay;

namespace GoopScripts.Cards
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