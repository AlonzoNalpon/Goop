/*!*********************************************************************
\file   HomeBase.cs
\author loh.j\@digipen.edu
\date   15 March 2024
\brief  
  Scene manager for deck viewer
 
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

    public void OnCreate()
    {
      //Serialization.SerialReader.IncrementLevel("./Assets/GameData/PlayerStats.sav");
      var statsInfo = Serialization.SerialReader.LoadPlayerState();
      statsInfo.SortDeck();
      m_levelToLoad = statsInfo.levelToLoad;

      // Console.WriteLine("Current Level: " + m_levelToLoad);
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
      if (m_levelToLoad == 4)
      {
        Utils.UpdateSprite((uint)m_mapID, "UI_Map_05");
      }
      if (m_levelToLoad >= 5)
      {
        Utils.UpdateSprite((uint)m_mapID, "UI_Map_06");
      }
      int counter = 0;
      foreach (var card in statsInfo.deckList)
      {
        Utils.SetTextComponent((int)Utils.GetChildEntity((uint)CardIDs[counter++], "Text"), "x" + card.Item2);
      }
    }

    static public int GetLevelToLoad()
    {
      return m_levelToLoad;
    }

    public void OnUpdate(double deltaTime)
    {
      if (Utils.IsKeyTriggered(Input.KeyCode.L))
      {
        if (m_levelToLoad < 5)
        {
          Serialization.SerialReader.IncrementLevel();
          OnCreate();
        }
      }
    }
  }
}
