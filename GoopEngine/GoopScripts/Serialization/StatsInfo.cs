using GoopScripts.Cards;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Serialization
{
  public struct StatsInfo
  {
    public List<Tuple<CardBase.CardID, uint>> deckList;
    public int health, maxHealth;
    public int levelToLoad;
  }
}
