#define IMGUI_ENABLED

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;
using GoopScripts.Mono;

namespace GoopScripts.Gameplay
{

  public class DeckManager
  {
    static readonly double CARD_WIDTH = 214.0;
    static readonly double PLAYER_HAND_WIDTH = CARD_WIDTH * 5 + 300.0;
    static readonly Vec3<double> HAND_START_POS = new Vec3<double>(-PLAYER_HAND_WIDTH / 2.0 - 60.0, -350.0, 10.0);

    static public readonly int STARTING_CARDS = 5, MAX_CARDS = 5;
    static readonly int QUEUE_SIZE = 3;

    public Deck m_deck;
    public List<CardBase.CardID> m_discard;
    // <CardID, EntityID>
    public ValueTuple<CardBase.CardID, uint>[] m_queue;
    public List<ValueTuple<CardBase.CardID, uint>> m_hand;

#if IMGUI_ENABLED
    public CardBase.CardID[] m_discardDisplay;
#endif

    public DeckManager()
    {
      m_deck = new Deck();
      m_discard = new List<CardBase.CardID>();
      m_hand = new List<ValueTuple<CardBase.CardID, uint>>();
      m_queue = new ValueTuple<CardBase.CardID, uint>[QUEUE_SIZE];

      for (int i = 0; i < QUEUE_SIZE; i++)
      {
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
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
//    public void Init(CharacterType type)
//    {
//      switch (type)
//      {
//        case CharacterType.PLAYER:
//          m_deck.AddCard(CardBase.CardID.LEAH_BEAM, 4);
//          m_deck.AddCard(CardBase.CardID.LEAH_STRIKE, 4);
//          m_deck.AddCard(CardBase.CardID.LEAH_SHIELD, 4);
//          m_deck.AddCard(CardBase.CardID.SPECIAL_FLASHBANG);
//          break;
//        case CharacterType.BASIC_ENEMY:
//          m_deck.AddCard(CardBase.CardID.BASIC_ATTACK, 4);
//          m_deck.AddCard(CardBase.CardID.BASIC_SHIELD, 3);
//          m_deck.AddCard(CardBase.CardID.SPECIAL_SCREECH);
//          break;
//        case CharacterType.BOSS_P1:
//          m_deck.AddCard(CardBase.CardID.DAWSON_BEAM, 2);
//          m_deck.AddCard(CardBase.CardID.DAWSON_SWING, 2);
//          m_deck.AddCard(CardBase.CardID.DAWSON_SHIELD, 5);
//          m_deck.AddCard(CardBase.CardID.SPECIAL_CHARGEUP);
//          break;
//        case CharacterType.BOSS_P2:
//          m_deck.AddCard(CardBase.CardID.DAWSON_BEAM, 3);
//          m_deck.AddCard(CardBase.CardID.DAWSON_SWING, 3);
//          m_deck.AddCard(CardBase.CardID.DAWSON_SHIELD, 6);
//          m_deck.AddCard(CardBase.CardID.SPECIAL_CHARGEUP, 2);
//          m_deck.AddCard(CardBase.CardID.SPECIAL_TIMEWRAP);
//          break;
//        default:
//#if (DEBUG)
//          //Console.WriteLine("Unable to create deck of type: " + type.ToString());
//#endif
//          break;
//      }

//      if (m_deck.Size() < STARTING_CARDS)
//      {
//#if (DEBUG)
//        Console.WriteLine("Deck size of " + m_deck.Size() + " lesser than STARTING_CARDS (" + STARTING_CARDS + ")");
//#endif
//        return;
//      }

//      m_deck.Shuffle();
//    }

    public void AlignHandCards()
    {
      double padding = (PLAYER_HAND_WIDTH - (double)m_hand.Count * CARD_WIDTH) / (double)(m_hand.Count + 1);
      Vec3<double> cardPos = HAND_START_POS;
      cardPos.X += padding + CARD_WIDTH / 2.0;
      foreach (var c in m_hand)
      {
        Utils.SetPosition(c.Item2, cardPos);
        cardPos.X += padding + CARD_WIDTH;
      }
    }

    /*!*********************************************************************
		\brief
		  Queues a card given the index of the card in the hand.
    \return
      The index of the queue it queued into and m_queue.Length otherwise.
		************************************************************************/
    public int Queue(int index)
    {
      int i = 0;
      for (; i < m_queue.Length; ++i)
      {
        if (m_queue[i].Item1 == CardBase.CardID.NO_CARD)
        {
          m_queue[i] = m_hand[index];
          break;
        }
      }
#if (DEBUG)
      //Console.WriteLine("Queuing " + m_hand[index].ToString() + " from hand\n");
#endif

      m_hand.RemoveAt(index);
      
      return i;
    }

    public void Unqueue(int index)
    {
      m_hand.Add(m_queue[index]);

#if (DEBUG)
      //Console.WriteLine("Returning " + m_queue[index].ToString() + " to hand\n");
#endif
      m_queue[index] = (CardBase.CardID.NO_CARD, uint.MaxValue);
    }

    /*!*********************************************************************
		\brief
		  Draws the top card of the deck. If the deck is empty, the discard
      pile will be reshuffled into the deck. If the hand is full, the top
      card of the deck will be destroyed or "burnt". Otherwise, the card is
      added to the hand.
    \return
      -1 if the card was destroyed and the index in the hand that the card 
      was added to otherwise
		************************************************************************/
    public int Draw()
    {
      // if empty, shuffle discard back
      if (m_deck.Empty())
      {
        m_deck.Restore(ref m_discard);
        m_deck.Shuffle();
#if (DEBUG)
        //Console.WriteLine("Deck Empty! Reshuffling");
        //Console.WriteLine();
#endif
      }

      // if hand full, destroy the top card
      if (m_hand.Count == MAX_CARDS)
      {
#if (DEBUG)
        //Console.WriteLine("Hand Full! Burnt " + m_deck.m_drawOrder.First().ToString());
        //Console.WriteLine();
#endif
        m_deck.BurnTop();
        return -1;
      }

#if (DEBUG)
      //Console.WriteLine("Drew " + m_deck.m_drawOrder.First().ToString());
      //Console.WriteLine();
#endif

      m_hand.Add((m_deck.Draw(), uint.MaxValue));
      return m_hand.Count - 1;     
    }
    
    /*!*********************************************************************
		\brief
		  Sends all cards in the queue to the discard pile.
		************************************************************************/
    public void DiscardQueue()
    {
      for (int i = 0; i < m_queue.Length; ++i)
      {
        if (m_queue[i].Item1 == CardBase.CardID.NO_CARD) { continue; }

#if (DEBUG)
        //Console.WriteLine("Discarding " + m_queue[i].ToString());
#endif
        m_discard.Add(m_queue[i].Item1);

        // destroy the entity if it exists
        if (m_queue[i].Item2 != uint.MaxValue)
        {
          Utils.DestroyEntity(m_queue[i].Item2);
        }
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
      }
    }

    public bool IsEntityInQueue(uint entity)
    {
      return m_queue.Any(card => card.Item2 == entity);
    }

    public bool IsEntityInHand(uint entity)
    {
      return m_hand.Any(card => card.Item2 == entity);
    }

    public bool IsQueueFull()
    {
      return !m_queue.Any(card => card.Item1 == CardBase.CardID.NO_CARD);
    }

    /*!*********************************************************************
		\brief
		  Empties the discard pile, card queue and hand.
		************************************************************************/
    public void Clear()
    {
      m_discard.Clear();

      for (int i = 0; i < m_hand.Count; i++)
      {
        if (m_hand[i].Item2 != uint.MaxValue)
        {
          Utils.DestroyEntity(m_hand[i].Item2);
        }
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
      }
      m_hand.Clear();

      for (int i = 0; i < m_queue.Length; i++)
      {
        if (m_queue[i].Item2 != uint.MaxValue)
        {
          Utils.DestroyEntity(m_queue[i].Item2);
        }
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
      }
    }
  }
}
