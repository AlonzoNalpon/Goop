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

      for (int i = 0; i < QUEUE_SIZE; i++)
      {
        m_queue[i] = CardBase.CardID.NO_CARD;
      }

#if IMGUI_ENABLED
      m_discardDisplay = m_discard.ToArray();
#endif
    }

    /*!*********************************************************************
		\brief
		  Initializes the deck for the start of a battle. Shuffles the deck and
      adds cards to the hand equal to STARTING_CARDS.
		************************************************************************/
    public void Init(CharacterType type)
    {
      switch (type)
      {
        case CharacterType.PLAYER:
          m_deck.AddCard(CardBase.CardID.LEAH_BEAM, 4);
          m_deck.AddCard(CardBase.CardID.LEAH_STRIKE, 4);
          m_deck.AddCard(CardBase.CardID.LEAH_SHIELD, 4);
          m_deck.AddCard(CardBase.CardID.SPECIAL_FLASHBANG);
          break;
        case CharacterType.BASIC_ENEMY:
          m_deck.AddCard(CardBase.CardID.BASIC_ATTACK, 4);
          m_deck.AddCard(CardBase.CardID.BASIC_SHIELD, 3);
          m_deck.AddCard(CardBase.CardID.SPECIAL_SCREECH);
          break;
        case CharacterType.BOSS_P1:
          m_deck.AddCard(CardBase.CardID.DAWSON_BEAM, 2);
          m_deck.AddCard(CardBase.CardID.DAWSON_SWING, 2);
          m_deck.AddCard(CardBase.CardID.DAWSON_SHIELD, 5);
          m_deck.AddCard(CardBase.CardID.SPECIAL_CHARGEUP);
          break;
        case CharacterType.BOSS_P2:
          m_deck.AddCard(CardBase.CardID.DAWSON_BEAM, 3);
          m_deck.AddCard(CardBase.CardID.DAWSON_SWING, 3);
          m_deck.AddCard(CardBase.CardID.DAWSON_SHIELD, 6);
          m_deck.AddCard(CardBase.CardID.SPECIAL_CHARGEUP, 2);
          m_deck.AddCard(CardBase.CardID.SPECIAL_TIMEWRAP);
          break;
        default:
#if (DEBUG)
          Console.WriteLine("Unable to create deck of type: " + type.ToString());
#endif
          break;
      }

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
        m_hand[i] = m_deck.Draw();
      }

      Console.WriteLine("\nDraw Order");
      foreach (CardBase.CardID card in m_deck.m_drawOrder)
      {
        Console.WriteLine(card.ToString());
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
      Console.WriteLine("Queuing " + m_hand[index].ToString() + " from hand\n");
#endif

      m_hand[index] = CardBase.CardID.NO_CARD;
    }

    public void Unqueue(int index)
    {
      // find empty slot and replace with card
      for (int i = 0; i < MAX_CARDS; ++i)
      {
        if (m_hand[i] == CardBase.CardID.NO_CARD)
        {
          m_hand[i] = m_queue[index];
        }
      }

#if (DEBUG)
      Console.WriteLine("Returning " + m_queue[index].ToString() + " to hand\n");
#endif
      m_queue[index] = CardBase.CardID.NO_CARD;
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

      // find empty slot and replace with drawn card
      int i = 0;
      for (; i < MAX_CARDS; ++i)
      {
        if (m_hand[i] == CardBase.CardID.NO_CARD)
        {
#if (DEBUG)
          Console.WriteLine("Drew " + m_deck.m_drawOrder.First().ToString());
          Console.WriteLine();
#endif
          m_hand[i] = m_deck.Draw();
          break;
        }
      }

      // if hand full, destroy the top card
      if (i == MAX_CARDS)
      {
#if (DEBUG)
        Console.WriteLine("Hand Full! Burnt " + m_deck.m_drawOrder.First().ToString());
        Console.WriteLine();
#endif
        m_deck.BurnTop();
      }
    }
    
    /*!*********************************************************************
		\brief
		  Sends all cards in the queue to the discard pile.
		************************************************************************/
    public void DiscardQueue()
    {
      for (int i = 0; i < m_queue.Length; ++i)
      {
        if (m_queue[i] == CardBase.CardID.NO_CARD) { continue; }

#if (DEBUG)
        Console.WriteLine("Discarding " + m_queue[i].ToString());
#endif
        m_discard.Add(m_queue[i]);
        m_queue[i] = CardBase.CardID.NO_CARD;
      }
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
