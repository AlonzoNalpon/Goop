using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts
{
	internal abstract class Stats
	{
		public uint health;
		// List of card IDs
		public List<CardBase.CardID> deck;
		// List of cards in the queue
		public List<CardBase.CardID> cardQueue;
	}
}
