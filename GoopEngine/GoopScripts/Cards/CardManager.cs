using GoopScripts.Gameplay;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;


namespace GoopScripts.Cards
{
	internal class CardManager
	{
		// Supposed to be a bunch of polymorphic cards of tye card base
		// the card objects are set via c++ interface
		List<CardBase> m_cards;
		public List<CardBase> Cards { get { return m_cards; } }
		// Function to pass m_cards.ToArray() to c++ 
		// public void ...()

		public CardManager()
		{
			m_cards = new List<CardBase>((int)CardBase.CardID.TOTAL_CARDS);
		}
	}
}
