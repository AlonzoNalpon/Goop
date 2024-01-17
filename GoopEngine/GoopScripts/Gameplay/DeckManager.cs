﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.Tracing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;
using GoopScripts.Mono;

namespace GoopScripts.Gameplay
{
  public class DeckManager
  {
    static readonly int STARTING_CARDS = 5, MAX_CARDS = 5;
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

    public DeckManager(Deck deck)
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

      m_hand.RemoveAt(index);
    }

    public void Draw()
    {
      // if empty, shuffle discard back
      if (m_deck.Empty())
      {
        m_deck.Restore(ref m_discard);
      }


      // if hand full, destroy the top card
      if (m_hand.Count >= MAX_CARDS)
      {
        m_deck.BurnTop();
      }
      else  // else draw card
      {
        m_hand.Add(m_deck.Draw());
#if (DEBUG)
        Console.WriteLine("Drew  " + m_hand.Last().ToString());
        Console.WriteLine();
#endif
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
      m_hand.Clear();
      for (int i = 0; i < m_queue.Length; i++)
      {
        m_queue[i] = CardBase.CardID.NO_CARD;
      }
    }
  }
}