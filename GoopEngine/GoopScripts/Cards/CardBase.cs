using GoopScripts.Mono;

namespace GoopScripts
{
  abstract class CardBase : MonoBehaviour
  {
		public enum CardID
		{
			// Basic enemy cards
			BASIC_LUNGE_ATTACK,
			BASIC_NORMAL_ATTACK,

			BASIC_NORMAL_BLOCK,

			BASIC_BUFF_2X_ATTACK,

			// Player cards
			PLAYER_BEAM_ATTACK,
			PLAYER_EMPOWERED_ATTACK,
			PLAYER_EMPOWERED_SWING,

			PLAYER_NORMAL_BLOCK,

			PLAYER_DEBUFF_FLASH_BANG,
			PLAYER_BUFF_SMOKESCREEN,
			PLAYER_BUFF_RAGE,
			PLAYER_EMPOWERED_BEAM_ATTACK,

			// Dawson enemy cards
			DAWSON_BEAM_ATTACK,
			DAWSON_EMPOWERED_ATTACK,

			DAWSON_NORMAL_BLOCK,

			DAWSON_DEBUFF_CHARGE_ATTACK,
			DAWSON_DEBUFF_SPEED_UP_QUEUE,

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
