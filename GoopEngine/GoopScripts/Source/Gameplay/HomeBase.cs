/*!*********************************************************************
\file   HomeBase.cs
\author loh.j\@digipen.edu
\date   15-March-2024
\brief  
  Script for the game's deck viewer screen, which displays the list
  of cards in the player's deck as well as a map showing the player's
  current level.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Cards;
using GoopScripts.Mono;
using GoopScripts.Serialization;
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
    public static int m_levelToLoad = 0;

    public string playerSavePath;
    public int[] CardIDs = new int[6];
    public int m_mapID;

    /*!*********************************************************************
    \brief
      OnCreate function to set the map based on the current level of the
      player.
    ************************************************************************/
    public void OnCreate()
    {
      //Serialization.SerialReader.IncrementLevel("./Assets/GameData/PlayerStats.sav");
      var statsInfo = Serialization.SerialReader.LoadPlayerState();
      statsInfo.SortDeck();
      m_levelToLoad = statsInfo.levelToLoad;

      if (m_levelToLoad <= 0)
      {
        Utils.UpdateSprite((uint)m_mapID, "UI_Map_01");
      }
      if (m_levelToLoad == 1)
      {
        Utils.UpdateSprite((uint)m_mapID, "UI_Map_02");
      }
      if (m_levelToLoad == 2)
      {
        Utils.UpdateSprite((uint)m_mapID, "UI_Map_03");
      }
      if (m_levelToLoad == 3)
      {
        Utils.UpdateSprite((uint)m_mapID, "UI_Map_04");
      }
      if (m_levelToLoad >= 4)
      {
        Utils.UpdateSprite((uint)m_mapID, "UI_Map_05");
      }

      foreach (var card in statsInfo.deckList)
      {
        int index = card.Item1 - CardID.LEAH_BEAM;
        Utils.SetTextComponent((int)Utils.GetChildEntity((uint)CardIDs[index], "Text"), "x" + card.Item2);
      }
    }

    /*!*********************************************************************
    \brief
      Returns the level to be loaded
    \return
      The level to load
    ************************************************************************/
    static public int GetLevelToLoad()
    {
      return m_levelToLoad;
    }
  }
}
