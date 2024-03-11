using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Gameplay;

namespace GoopScripts.Button
{
  public class RewardConfirm : TextButtonBase
  {
    public Stats m_playerStats;
    public RewardConfirm()
    {
      m_playerStats = (Stats)Utils.GetScript("Player", "Stats");
    }

    public override void OnClick(uint entity)
    {
      for (int i = 0; i < RewardManager.m_selectedCards.Count; ++i)
      {
        m_playerStats.m_deckMngr.m_deck.AddCard((Cards.CardBase.CardID)RewardManager.m_selectedCards[i]);
      }

      RewardManager.m_selectedCards.Clear();
    }
  }
}
