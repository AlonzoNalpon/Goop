#define IMGUI_ENABLE

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
  public class Deck
  {
    public CardBase.CardID[] m_cards; // the actual deck
    public List<CardBase.CardID> m_drawOrder; // the order of cards to draw

#if IMGUI_ENABLE
    // to display in inspector
    public CardBase.CardID[] m_drawOrderDisplay;
#endif

    Random m_rand;

    public Deck()
    {
      m_rand = new Random();
      m_cards = new CardBase.CardID[0];
      m_drawOrder = new List<CardBase.CardID>();

#if IMGUI_ENABLE
      m_drawOrderDisplay = m_drawOrder.ToArray();
#endif
    }

    public Deck(List<CardBase.CardID> cardList)
    {
      m_rand = new Random();
      m_cards = cardList.ToArray();
      m_drawOrder = new List<CardBase.CardID>(m_cards.ToList());

#if IMGUI_ENABLE
      m_drawOrderDisplay = m_drawOrder.ToArray();
#endif
    }

    public Deck(Deck deck)
    {
      m_rand = new Random();
      m_cards = new CardBase.CardID[deck.m_cards.Length];
      deck.m_cards.CopyTo(m_cards, 0);
      m_drawOrder = new List<CardBase.CardID>(deck.m_drawOrder);

#if IMGUI_ENABLE
      m_drawOrderDisplay = m_drawOrder.ToArray();
#endif
    }

    /*!*********************************************************************
		\brief
		  Adds a card into the deck. The number of copies can be specified as
      the 2nd argument.
    \param card
      The CardID of the card to add
    \param count
      The number of copies
		************************************************************************/
    public void AddCard(CardBase.CardID card, uint count = 1)
    {
      int oldSize = m_cards.Length;
      // create new array and copy into it
      CardBase.CardID[] newCards = new CardBase.CardID[oldSize + count];
      m_cards.CopyTo(newCards, 0);

      // add new cards to the back of array
      for (int i = oldSize; i < newCards.Length; ++i)
      {
        newCards[i] = card;
        m_drawOrder.Add(card);
#if (DEBUG)
        //Console.WriteLine("Added " + card.ToString() + " to deck");
#endif
      }

      m_cards = newCards;

#if IMGUI_ENABLE
    m_drawOrderDisplay = m_drawOrder.ToArray();
#endif
    }

    /*!*********************************************************************
		\brief
		  Draws the top card of the deck. It is removed from the deck.
		\return
			The ID of the card drawn
		************************************************************************/
    public CardBase.CardID Draw()
    {
      if (Empty()) { return CardBase.CardID.NO_CARD; }

      CardBase.CardID cardDrawn = m_drawOrder.First();

#if (DEBUG)
      //Console.WriteLine("Draw Card: " + cardDrawn.ToString());
#endif

      m_drawOrder.RemoveAt(0);

#if IMGUI_ENABLE
      m_drawOrderDisplay = m_drawOrder.ToArray();
#endif

      return cardDrawn;
    }

    /*!*********************************************************************
		\brief
      Removes the top card of the deck.
		************************************************************************/
    public void BurnTop()
    {
      if (Empty()) { return; }

      m_drawOrder.RemoveAt(0);

#if IMGUI_ENABLE
      m_drawOrderDisplay = m_drawOrder.ToArray();
#endif
    }

    /*!*********************************************************************
		\brief
      Shuffles the deck with the Fisher-Yates algorithm
		************************************************************************/
    public void Shuffle()
    {
      int i = Size();
      while (i > 1)
      {
        int idx = m_rand.Next(0, i);
        --i;
        CardBase.CardID temp = m_drawOrder[idx];
        m_drawOrder[idx] = m_drawOrder[i];
        m_drawOrder[i] = temp;
      }

#if IMGUI_ENABLE
      m_drawOrderDisplay = m_drawOrder.ToArray();
#endif
    }

    /*!*********************************************************************
		\brief
      Adds a list of cards back into the deck. The pile is cleared.
    \param pile
      The list of CardIDs to return back to the deck
		************************************************************************/
    public void Restore(ref List<CardBase.CardID> pile)
    {
      m_drawOrder.AddRange(pile);
      pile.Clear();

#if IMGUI_ENABLE
      m_drawOrderDisplay = m_drawOrder.ToArray();
#endif
    }

    /*!*********************************************************************
		\brief
		  Gets the number of cards in the deck
		\return
			The number of cards in the deck
		************************************************************************/
    public int Size()
    {
      return m_drawOrder.Count;
    }

    public bool Empty()
    {
      return m_drawOrder.Count == 0;
    }
  }
}
