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
    static readonly int REWARD_POOL_COUNT = 6, NUM_CHOICES = 5;
    static readonly double WIDTH = 1100;
    static List<uint> m_selectedEntityIDs;
    // public int m_hover;

    Random rng = new Random();
    public CardBase.CardID[] m_rewardPool = new CardBase.CardID[REWARD_POOL_COUNT];
    public int[] m_dropRates = new int[REWARD_POOL_COUNT];
    static uint[] m_cardEntities = new uint[NUM_CHOICES];

    public RewardManager()
    {
      m_selectedEntityIDs = new List<uint>();
    }

    public void OnCreate()
    {
      uint hover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      Utils.SetIsActiveEntity(hover, false);
      RewardCard.m_cardHover = (uint)hover;
      for (int i = 0; i < NUM_CHOICES; ++i)
      {
        CardBase.CardID generatedCard = GetChosenCard(rng.Next(1, 101));
        m_rewardPool[i] = generatedCard;
        uint parent = Utils.SpawnPrefab(CardManager.m_cardPrefabs[generatedCard] + "_Hover", new Vec3<double>(-WIDTH * 0.5 + (WIDTH / (NUM_CHOICES - 1)) * i, 0, 0));
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

      // Console.WriteLine("Chance: " + chance + " | Card: " + m_rewardPool[i]);
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
        selectedCards.Add(m_rewardPool[Array.IndexOf(m_cardEntities, e)]);
      }
      Serialization.SerialReader.AddCardsToDeck(selectedCards, "./Assets/GameData/PlayerStats.sav");
    }

    static public bool IsFull()
    {
      return m_selectedEntityIDs.Count > 3;
    }
  }
}
