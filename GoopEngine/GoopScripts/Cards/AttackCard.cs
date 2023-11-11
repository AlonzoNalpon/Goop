using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;
using static GoopScripts.BuffManager;

namespace GoopScripts.Cards
{
  internal class AttackCard : CardBase
  {
		public override void Play(uint? actionVal, uint? source, uint? target)
		{
			uint targetHp = GetHealth((uint)target);
			targetHp -= (uint)actionVal;
			SetHealth((uint)target, targetHp);
		}
	}
}