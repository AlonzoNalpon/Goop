using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;

namespace GoopScripts.Gameplay
{
  public class DeckManager
  {
    static readonly int STARTING_CARDS = 5;
    static readonly int QUEUE_SIZE = 3;

    public Deck m_deck;
    public List<CardBase.CardID> m_discard, m_hand;
    public CardBase.CardID[] m_queue;

    public DeckManager()
    {
      m_deck = new Deck();
      m_discard = new List<CardBase.CardID>();
      m_hand = new List<CardBase.CardID>();
      m_queue = new CardBase.CardID[QUEUE_SIZE];
    }

    public DeckManager(ref Deck deck)
    {
      m_deck = deck;
      m_discard = new List<CardBase.CardID>();
      m_hand = new List<CardBase.CardID>();
      m_queue = new CardBase.CardID[QUEUE_SIZE];
    }

    /*!*********************************************************************
		\brief
		  Initializes the deck for the start of a battle. Shuffles the deck and
      adds cards to the hand equal to STARTING_CARDS.
		************************************************************************/
    public void Init()
    {
      if (m_deck.Size() < STARTING_CARDS)
      {
#if (DEBUG)
        Console.WriteLine("Deck size of " + m_deck.Size() + " lesser than STARTING_CARDS (" + STARTING_CARDS + ")");
#endif
        return;
      }

      m_deck.Shuffle();
      for(int i = 0; i < STARTING_CARDS; ++i)
      {
        m_hand.Add(m_deck.Draw());
      }
    }

    //public void Queue()

    /*!*********************************************************************
		\brief
		  Empties the discard pile, card queue and hand.
		************************************************************************/
    public void Clear()
    {
      m_discard.Clear();
      m_hand.Clear();
      for (int i = 0; i < m_queue.Length; i++)
      {
        m_queue[i] = CardBase.CardID.NO_CARD;
      }
    }
  }
}
