/*!*********************************************************************
\file   RewardManager.cs
\author loh.j\@digipen.edu
\date   15 March 2024
\brief  
  Script for managing the reward screen.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Cards;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class RewardManager
  {
    static readonly int REWARD_POOL_COUNT = 6, NUM_CHOICES = 5, MAX_SELECT = 3;
    static readonly double WIDTH = 1100;
    static readonly double Y_OFFSET = 40;
    static List<uint> m_selectedEntityIDs;
    // public int m_hover;

    Random rng = new Random();
    public CardBase.CardID[] m_rewardPool = new CardBase.CardID[REWARD_POOL_COUNT];
    CardBase.CardID[] m_generatedPool = new CardBase.CardID[REWARD_POOL_COUNT];
    public int[] m_dropRates = new int[REWARD_POOL_COUNT];
    public string playerSavePath;
    static uint[] m_cardEntities = new uint[NUM_CHOICES];


    public RewardManager()
    {
      m_selectedEntityIDs = new List<uint>();
    }

    public void OnCreate()
    {
      uint hover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      var statsInfo = Serialization.SerialReader.LoadPlayerState(playerSavePath);
      int m_levelToLoad = statsInfo.levelToLoad;
      Utils.SetIsActiveEntity(hover, false);
      RewardCard.m_cardHover = (uint)hover;
      if (m_levelToLoad <= 0) // tutorial
      {
        for (int i = 0; i < MAX_SELECT; ++i)
        {
          CardBase.CardID generatedCard = (CardBase.CardID)4 + i;
          m_generatedPool[i] = generatedCard;
          uint parent = Utils.SpawnPrefab(CardManager.m_cardPrefabs[generatedCard] + "_Hover", new Vec3<double>(-(WIDTH / NUM_CHOICES * MAX_SELECT) * 0.5 + ((WIDTH / NUM_CHOICES * MAX_SELECT) / (MAX_SELECT - 1)) * i, Y_OFFSET, 0));
          m_cardEntities[i] = Utils.GetChildEntity(parent, "Base");
          Utils.SetScript(m_cardEntities[i], "RewardCard");
        }
        return;
      }
      for (int i = 0; i < NUM_CHOICES; ++i)
      {
        int chance = rng.Next(1, 101);
        CardBase.CardID generatedCard = GetChosenCard(chance);
        m_generatedPool[i] = generatedCard;
        Console.WriteLine("Card "+ i + ": " + chance + " with this generated: " + generatedCard);
        uint parent = Utils.SpawnPrefab(CardManager.m_cardPrefabs[generatedCard] + "_Hover", new Vec3<double>(-WIDTH * 0.5 + (WIDTH / (NUM_CHOICES - 1)) * i, Y_OFFSET, 0));
        m_cardEntities[i] = Utils.GetChildEntity(parent, "Base");
        Utils.SetScript(m_cardEntities[i], "RewardCard");
      }
    }

    CardBase.CardID GetChosenCard(int chance)
    {
      int i = 0;
      for (int currentChance = 0; i < REWARD_POOL_COUNT; ++i)
      {
        currentChance += m_dropRates[i];
        if (chance <= currentChance)
        {
          break;
        }
      }

      Console.WriteLine("Chance: " + chance + " | Card: " + m_rewardPool[i]);
      return m_rewardPool[i];
    }

    static public void CardSelected(uint entity)
    {
      m_selectedEntityIDs.Add(entity);
    }

    static public void CardUnselected(uint entity)
    {
      m_selectedEntityIDs.Remove(entity);
    }

    public void AddCardsToDeck()
    {
      List<CardBase.CardID> selectedCards = new List<CardBase.CardID>();
      foreach (uint e in m_selectedEntityIDs)
      {
        selectedCards.Add(m_generatedPool[Array.IndexOf(m_cardEntities, e)]);
      }
      Serialization.SerialReader.AddCardsToDeck(selectedCards, "./Assets/GameData/PlayerStats.sav");
    }

    static public bool IsFull()
    {
      return m_selectedEntityIDs.Count >= 3;
    }
  }
}
