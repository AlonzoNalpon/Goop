using GoopScripts.Cards;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Gameplay
{
  public class HomeBase
  {
    readonly int NUM_ROWS = 2, NUM_COLS = 5;
    //int page = 0;

    List<CardBase.CardID> m_tempDeck = new List<CardBase.CardID>();
    Dictionary<CardBase.CardID, int> m_cards = new Dictionary<CardBase.CardID, int>();
    // List<CardBase.CardID> m_sortedDeck = new List<CardBase.CardID>();


    public HomeBase()
    { 
      Random random = new Random();
      for (int i = 0; i < 50; ++i)
      {
        m_tempDeck.Add((CardBase.CardID)random.Next(1, (int)CardBase.CardID.TOTAL_CARDS));
        Console.WriteLine("Added " + m_tempDeck.Last());
      }
      GetUniqueCards(m_tempDeck);

      for (int i = 0; i < m_cards.Count; ++i)
      {

      }
      for (int i = 0; i < NUM_ROWS; ++i)
      {
        for (int j = 0; j < NUM_COLS; ++j)
        {
          Console.WriteLine(m_cards.ElementAt(i * NUM_COLS + j).Key.ToString() + m_cards.ElementAt(i * NUM_COLS + j).Value);
        }
      }

    }

    public void GetUniqueCards(List<CardBase.CardID> deck)
    {
      for (int i = 0; i < deck.Count; ++i)
      {
        if (m_cards.ContainsKey(deck[i]))
        {
          ++m_cards[deck[i]];
        }
        else
        {
          m_cards[deck[i]] = 1;
        }
      }
    }
  }
}
