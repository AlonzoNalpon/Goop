
using System;
using System.Collections.Generic;
using System.Deployment.Application;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts
{
  internal class Buff
  {
    public enum BuffType
    {
      ADDITIVE,
      SUBTRACTIVE,
      MULTIPLICATIVE,
      DIVISIVE
    }

    BuffType m_type;
    int m_value;
    int m_turns;

    // Getters
    public BuffType type { get { return m_type; } }
    public int value { get { return m_value; } }
    public int turns { get { return m_turns; } set { m_turns = value; } }

    public Buff(BuffType type, int value, int turns)
    {
      m_type = type;
      m_value = value;
      m_turns = turns; 
    }
  }

  internal class BuffManager
  {    
    private List<Buff> m_buffs;
    private List<Buff> m_debuffs;

    public BuffManager()
    {
      m_buffs = new List<Buff>();
      m_debuffs = new List<Buff>();
    }

    public void AddBuff(Buff buff)
    {
      m_buffs.Add(buff);
    }
    public void AddDebuff(Buff buff)
    {
      m_debuffs.Add(buff);
    }

    public List<int> CalulateBuffs(int baseValue)
    {
      List<int> result = new List<int>();

      foreach (Buff buff in m_buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.ADDITIVE:
            result.Add(buff.value);
            break;
          case Buff.BuffType.SUBTRACTIVE:
            result.Add(-buff.value);
            break;
          case Buff.BuffType.MULTIPLICATIVE:
            result.Add(baseValue * buff.value);
            break;
          case Buff.BuffType.DIVISIVE:
            result.Add(baseValue / buff.value);
            break;
        }
      }

      return result;
    }

    public List<int> CalulateDebuffs(int baseValue)
    {
      List<int> result = new List<int>();

      foreach (Buff buff in m_buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.ADDITIVE:
            result.Add(buff.value);
            break;
          case Buff.BuffType.SUBTRACTIVE:
            result.Add(-buff.value);
            break;
          case Buff.BuffType.MULTIPLICATIVE:
            result.Add(baseValue * buff.value);
            break;
          case Buff.BuffType.DIVISIVE:
            result.Add(baseValue / buff.value);
            break;
        }
      }

      return result;
    }

    public void StepTurn()
    {
      foreach (var buff in m_buffs)
      {
        buff.turns = buff.turns - 1;
      }
      m_buffs.RemoveAll(buff => buff.turns <= 0);

      foreach (var buff in m_debuffs)
      {
        buff.turns = buff.turns - 1;
      }
      m_debuffs.RemoveAll(buff => buff.turns <= 0);
    }
  }
}
