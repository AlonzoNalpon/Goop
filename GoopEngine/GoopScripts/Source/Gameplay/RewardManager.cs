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
    static public List<CardBase.CardID> m_selectedCards;
    static public List<uint> m_selectedEntityIDs;
    // public int m_hover;

    Random rng = new Random();
    public CardBase.CardID[] m_rewardPool = new CardBase.CardID[REWARD_POOL_COUNT];
    public int[] m_dropRates = new int[REWARD_POOL_COUNT];
    uint[] m_cardEntities = new uint[NUM_CHOICES];

    public RewardManager()
    {
      m_selectedCards = new List<CardBase.CardID>();
    }

    public void OnCreate()
    {
      uint hover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      Utils.SetIsActiveEntity(hover, false);
      for (int i = 0; i < NUM_CHOICES; ++i)
      {
        CardBase.CardID generatedCard = GetChosenCard(rng.Next(1, 101));
        m_cardEntities[i] = Utils.SpawnPrefab(CardManager.m_cardPrefabs[generatedCard] + "_Hover", new Vec3<double>(-WIDTH * 0.5 + (WIDTH / (NUM_CHOICES - 1)) * i, 0, 0));
        Utils.SetScript(Utils.GetChildEntity(m_cardEntities[i], "Base"), "RewardCard");
        RewardCard.m_cardHover = (uint)hover;
        RewardCard.m_type = generatedCard;
        RewardCard.m_entityID = m_cardEntities[i];
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

    static public void AddCardsToDeck()
    {
      foreach (var entity in m_selectedEntityIDs)
      {
        Console.WriteLine(entity);
        // Utils.GetScriptFromID(entity, "RewardCard").Deselect();
      }
    }
  }
}
