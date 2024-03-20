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
  }

  public struct EnemyStatsInfo
  {
    public string portrait, background, prefab;
    public List<Tuple<CardBase.CardID, uint>> deckList;
    public int health, maxHealth;
    public Gameplay.CharacterType characterType;
  }
}
