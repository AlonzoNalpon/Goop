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
    static int m_levelToLoad = 0;

    public string playerSavePath;
    public int[] CardIDs = new int[6];

    public void OnCreate()
    {
      Serialization.SerialReader.IncrementLevel("./Assets/GameData/PlayerStats.sav");
      var statsInfo = Serialization.SerialReader.LoadPlayerState(playerSavePath);
      m_levelToLoad = statsInfo.levelToLoad;
      Console.WriteLine(m_levelToLoad);
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
  }
}
