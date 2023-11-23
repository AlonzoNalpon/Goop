using GoopScripts.Mono;
using GoopScripts.Gameplay;

namespace GoopScripts.Cards
{
  public abstract class CardBase : MonoBehaviour
  {
		public enum CardID
		{
			NO_CARD,

			// Basic enemy cards
			BASIC_NORMAL_ATTACK,
			BASIC_NORMAL_SHIELD,

			BASIC_NORMAL_BUFF,

			// Player cards
			PLAYER_BEAM_ATTACK,
			PLAYER_STRIKE_ATTACK,

			PLAYER_NORMAL_SHIELD,

			PLAYER_DEBUFF_FLASH_BANG,
			PLAYER_BUFF_SMOKESCREEN,
			PLAYER_BUFF_RAGE,

			// Dawson enemy cards
			DAWSON_BEAM_ATTACK,
			DAWSON_STRIKE_ATTACK,

			DAWSON_NORMAL_SHIELD,

			DAWSON_BUFF_CHARGE_UP,
			DAWSON_DEBUFF_TIME_WARP,

			TOTAL_CARDS,
		}

		CardID cardID;
		float m_actionVal;
		int m_duration;

		public float Value { get { return m_actionVal; } set { m_actionVal = value; } }
		public int Duration { get { return m_duration; } set { m_duration = value; } }

		public CardBase(CardID cardID) { this.cardID = cardID; }

		// Defined by derived class
		public abstract void Play(ref Stats source, ref Stats target);
	}
}
