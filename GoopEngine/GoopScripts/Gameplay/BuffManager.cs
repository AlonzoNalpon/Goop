
using GoopScripts.Mono;
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
      // BUFFS
      INCREASE_ATK_DEALT,
      MULTIPLY_ATK_DEALT,
      INCREASE_BLOCK,
      DRAW_CARDS,

      // DEBUFFS
      REDUCE_ATK_DEALT,
      SKIP_TURN,
      BLIND,

      TOTAL_BUFFS
    }

    BuffType m_type;
    bool m_isDebuff;
    float m_value;
    int m_turns;
    readonly string m_displayText;

    // Getters
    public BuffType type { get { return m_type; } }
    public float value { get { return m_value; } }
    public int turns { get { return m_turns; } set { m_turns = value; } }

    public string GetDisplayText() {  return m_displayText; }
    public bool IsDebuff() {  return m_isDebuff; }  
    public Buff(BuffType type, float value, int turns)
    {
      m_type = type;
      m_value = value;
      m_turns = turns + 1; 

      switch (type)
      {
        case BuffType.INCREASE_ATK_DEALT:
          m_displayText = "+" + value + " Attack for " + turns + " turns";
          m_isDebuff = false;
          break;
        case BuffType.MULTIPLY_ATK_DEALT:
          m_displayText = "x" + value + " Attack for " + turns + " turns";
          m_isDebuff = false;
          break;
        case BuffType.INCREASE_BLOCK:
          m_displayText = "+" + value + " Block for " + turns + " turns";
          m_isDebuff = false;
          break;
        case BuffType.DRAW_CARDS:
          m_displayText = "Draw cards";
          m_isDebuff = false;
          break;
        case BuffType.REDUCE_ATK_DEALT:
          m_displayText = "-" + value + " Attack for " + turns + " turns";
          m_isDebuff = true;
          break;
        case BuffType.SKIP_TURN:
          m_displayText = "Turn skipped";
          m_isDebuff = true;
          break;
        case BuffType.BLIND:
          m_displayText = "50% chance of missing attack";
          m_isDebuff = true;
          break;
      }
    }
  }

  public class BuffManager
  {    
    private List<Buff> m_buffs;
    public List<Buff> Buffs { get { return m_buffs; } }

    public int buffsUI; 

    public BuffManager(int displayID)
    {
      m_buffs = new List<Buff>();
      buffsUI = displayID;
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

    public void RemoveBuff()
    {
      Random rng = new Random();
      int chance = rng.Next(0, m_buffs.Count - 1);
      m_buffs.RemoveAt(chance);
    }

    public void UpdateBuffsUI()
    {
      string txt = "";
      foreach (Buff buff in m_buffs)
      {
        txt += buff.GetDisplayText() + "\n";
      }
      Utils.SetTextComponent(buffsUI, txt);
    }
  }
}
