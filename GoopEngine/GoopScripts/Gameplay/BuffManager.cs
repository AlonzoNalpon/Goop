
using System;
using System.Collections.Generic;
using System.Deployment.Application;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// TODO need to rewrite how buff and debuff numbers are calculated

namespace GoopScripts.Gameplay
{
  public class Buff
  {
    public enum BuffType
    {
      INCREASE_ATK_DEALT,
      MULTIPLY_ATK_DEALT,
      REDUCE_DMG_TAKEN,
      SKIP_TURN,
      PENETRATE_SHIELD,
      DRAW_CARD,
    }

    BuffType m_type;
    float m_value;
    int m_turns;

    // Getters
    public BuffType type { get { return m_type; } }
    public float value { get { return m_value; } }
    public int turns { get { return m_turns; } set { m_turns = value; } }

    public Buff(BuffType type, float value, int turns)
    {
      m_type = type;
      m_value = value;
      m_turns = turns; 
    }
  }

  public class BuffManager
  {    
    private List<Buff> m_buffs;

    public List<Buff> Buffs { get { return m_buffs; } }

    public BuffManager()
    {
      m_buffs = new List<Buff>();
    }

    public void AddBuff(Buff buff)
    {
      m_buffs.Add(buff);
    }

    public void StepTurn()
    {
      foreach (var buff in m_buffs)
      {
        buff.turns -= 1;
      }
      m_buffs.RemoveAll(buff => buff.turns <= 0);
    }
  }
}
