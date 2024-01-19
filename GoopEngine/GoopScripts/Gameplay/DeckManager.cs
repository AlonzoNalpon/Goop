#define IMGUI_ENABLED

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;

namespace GoopScripts.Gameplay
{
  public class DeckManager
  {
    static readonly int STARTING_CARDS = 5, MAX_CARDS = 5;
    static readonly int QUEUE_SIZE = 3;

    public Deck m_deck;
    public List<CardBase.CardID> m_discard;
    public CardBase.CardID[] m_hand, m_queue;

#if IMGUI_ENABLED
    public CardBase.CardID[] m_discardDisplay;
#endif

    public DeckManager()
    {
      m_deck = new Deck();
      m_discard = new List<CardBase.CardID>();
      m_hand = new CardBase.CardID[MAX_CARDS];
      m_queue = new CardBase.CardID[QUEUE_SIZE];

#if IMGUI_ENABLED
      m_discardDisplay = m_discard.ToArray();
#endif
  }

  public DeckManager(Deck deck)
    {
      m_deck = new Deck(deck);
      m_discard = new List<CardBase.CardID>();
      m_hand = new CardBase.CardID[MAX_CARDS];
      m_queue = new CardBase.CardID[QUEUE_SIZE];

#if IMGUI_ENABLED
      m_discardDisplay = m_discard.ToArray();
#endif
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
#if (DEBUG)
        Console.WriteLine("Added " + m_deck.m_drawOrder.First().ToString() + " to starting hand");
#endif
        m_hand[i] = m_deck.Draw();
      }
    }
    public void Queue(int index)
    {
      for (int i = 0; i < m_queue.Count(); ++i)
      {
        if (m_queue[i] == CardBase.CardID.NO_CARD)
        {
          m_queue[i] = m_hand[index];
          break;
        }
      }
#if (DEBUG)
      Console.WriteLine("Queuing " + m_hand[index].ToString() + " from hand");
      Console.WriteLine();
#endif

      m_hand[index] = CardBase.CardID.NO_CARD;
    }

    public void Draw()
    {
      // if empty, shuffle discard back
      if (m_deck.Empty())
      {
        m_deck.Restore(ref m_discard);
        m_deck.Shuffle();
#if (DEBUG)
        Console.WriteLine("Deck Empty! Reshuffling");
        Console.WriteLine();
#endif
      }


      // if hand full, destroy the top card
      if (m_hand.Length >= MAX_CARDS)
      {
#if (DEBUG)
        Console.WriteLine("Hand Full! Burnt " + m_deck.m_drawOrder.First().ToString());
        Console.WriteLine();
#endif
        m_deck.BurnTop();
      }
      else  // else draw card
      {
#if (DEBUG)
        Console.WriteLine("Drew  " + m_deck.m_drawOrder.First().ToString());
        Console.WriteLine();
#endif

        // find empty slot and replace with drawn card
        for (int i = 0; i < MAX_CARDS; ++i)
        {
          if (m_hand[i] == CardBase.CardID.NO_CARD)
          {
            m_hand[i] = m_deck.Draw();
          }
        }

      }
    }

    public void Resolve()
    {
      // first compute base card values

      // then resolve combos


#if (DEBUG)
      Console.WriteLine("Resolving queue with: ");
      foreach (CardBase.CardID c in m_queue)
      {
        Console.WriteLine(c.ToString());
      }
      Console.WriteLine();
#endif
    }

    /*!*********************************************************************
		\brief
		  Empties the discard pile, card queue and hand.
		************************************************************************/
    public void Clear()
    {
      m_discard.Clear();

      for (int i = 0; i < MAX_CARDS; i++)
      {
        m_hand[i] = CardBase.CardID.NO_CARD;
      }

      for (int i = 0; i < m_queue.Length; i++)
      {
        m_queue[i] = CardBase.CardID.NO_CARD;
      }
    }
  }
}
