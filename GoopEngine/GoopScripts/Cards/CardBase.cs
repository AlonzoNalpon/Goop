using GoopScripts.Mono;

namespace GoopScripts.Cards
{
  abstract class CardBase : MonoBehaviour
  {
    public abstract void Play(uint? actionVal, uint? source, uint? target);

	}
}
