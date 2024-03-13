using GoopScripts.Cards;
using GoopScripts.Mono;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static GoopScripts.Cards.CardBase;

namespace GoopScripts.Gameplay
{
  public class HomeBase
  {
    List<CardBase.CardID> m_tempDeck = new List<CardBase.CardID>();
    static Dictionary<CardBase.CardID, int> m_cards = new Dictionary<CardBase.CardID, int>();
    public string playerSavePath;
    public int[] CardIDs = new int[6];

    public HomeBase()
    {
      
    }

    public void OnCreate()
    {
      var statsInfo = Serialization.SerialReader.LoadPlayerState(playerSavePath);
      foreach (var card in statsInfo.deckList)
      {
        for (int i = 0; i < card.Item2; ++i)
        {
          m_tempDeck.Add(card.Item1);
          Console.Write(card.Item1);
        }
      }
      GetUniqueCards(m_tempDeck);
      for (int i = 0; i < CardIDs.Length; ++i)
      {
        Utils.SetTextComponent((int)Utils.GetChildEntity((uint)CardIDs[i], "Text"), "x" + HomeBase.GetCount(CardBase.CardID.LEAH_BEAM + i));
      }
    }

    public void GetUniqueCards(List<CardBase.CardID> deck)
    {
      for (CardBase.CardID i = CardBase.CardID.LEAH_BEAM; i <= CardBase.CardID.SPECIAL_RAGE; ++i)
      {
        m_cards.Add(i, 0);
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
      Console.WriteLine(card);

      if (m_cards.ContainsKey(card))
      {
        return m_cards[card];
      }
      return 0;
    }
  }
}
