/*!*********************************************************************
\file   StatsInfo.cs
\author chengen.lau\@digipen.edu
\date   15-March-2024
\brief  Struct encapsulating the data required to load the Player
        and Enemy Stats object.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Cards;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Serialization
{
  public struct PlayerStatsInfo
  {
    public List<Tuple<CardBase.CardID, uint>> deckList;
    public int health, maxHealth;
    public int levelToLoad;

    public void SortDeck()
    {
      Dictionary<CardBase.CardID, uint> cardMap = new Dictionary<CardBase.CardID, uint>();
      foreach (var card in deckList)
      {
        if (cardMap.ContainsKey(card.Item1))
          cardMap[card.Item1] += card.Item2;
        else
          cardMap.Add(card.Item1, card.Item2);
      }

      deckList.Clear();
      foreach (var cardData in cardMap)
      {
        deckList.Add(new Tuple<CardBase.CardID, uint>(cardData.Key, cardData.Value));
      }
    }
  }

  public struct EnemyStatsInfo
  {
    public string portrait, background, prefab;
    public List<Tuple<CardBase.CardID, uint>> deckList;
    public int health, maxHealth;
    public Gameplay.CharacterType characterType;
  }
}
