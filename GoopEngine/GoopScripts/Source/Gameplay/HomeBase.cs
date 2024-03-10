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
    List<CardBase.CardID> m_tempDeck = new List<CardBase.CardID>();
    static Dictionary<CardBase.CardID, int> m_cards = new Dictionary<CardBase.CardID, int>();

    public HomeBase()
    { 
    }

    public void OnCreate()
    {
      Random random = new Random();
      for (int i = 0; i < 50; ++i)
      {
        m_tempDeck.Add((CardBase.CardID)random.Next((int)CardBase.CardID.LEAH_BEAM, (int)CardBase.CardID.SPECIAL_RAGE));
      }
      GetUniqueCards(m_tempDeck);
    }

    public void GetUniqueCards(List<CardBase.CardID> deck)
    {
      for (CardBase.CardID i = CardBase.CardID.LEAH_BEAM; i <= CardBase.CardID.SPECIAL_RAGE; ++i)
      {
        m_cards[i] = 0;
      }

      for (int i = 0; i < deck.Count; ++i)
      {
        if (m_cards.ContainsKey(deck[i]))
        {
          ++m_cards[deck[i]];
        }
      }
    }

    static public int GetCount(CardBase.CardID card)
    {
      if (m_cards.ContainsKey(card))
      {
        return m_cards[card];
      }
      return 0;
    }
  }
}
