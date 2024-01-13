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
    public List<CardBase.CardID> m_cards;
    readonly Random m_rand;

    public Deck()
    {
      m_rand = new Random();
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
      for (int i = 0; i < count; ++i)
      {
        m_cards.Add(card);
      }
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

      CardBase.CardID cardDrawn = m_cards.First();
      m_cards.RemoveAt(0);
      return cardDrawn;
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
        CardBase.CardID temp = m_cards[idx];
        m_cards[idx] = m_cards[i];
        m_cards[i] = temp;
      }
    }

    /*!*********************************************************************
		\brief
		  Gets the number of cards in the deck
		\return
			The number of cards in the deck
		************************************************************************/
    public int Size()
    {
      return m_cards.Count();
    }

    public bool Empty()
    {
      return m_cards.Count() == 0;
    }
  }
}
