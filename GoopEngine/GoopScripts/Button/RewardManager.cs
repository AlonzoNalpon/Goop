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
    static public List<uint> m_selectedCards;

    public RewardManager()
    {
      m_selectedCards = new List<uint>();
    }

      public void OnCreate()
    {
      RewardCard.m_cardHover = Utils.SpawnPrefab("CardHover", new Vec3<double>(0.0, 0.0, 5.0));
      Utils.SetIsActiveEntity(RewardCard.m_cardHover, false);
    }
  }
}
