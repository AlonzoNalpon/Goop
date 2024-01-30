
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
      IMMUNITY,

      // DEBUFFS
      REDUCE_ATK_DEALT,
      REDUCE_SHIELD,
      SKIP_TURN,
      BLEED,
      TOTAL_BUFFS
    }

    BuffType m_type;
    bool m_isDebuff;
    float m_value;
    string m_label;
    int m_turns;
    readonly string m_displayText;

    // Getters
    public BuffType type { get { return m_type; } }
    public float value { get { return m_value; } }
    public int turns { get { return m_turns; } set { m_turns = value; } }

    public string Label { get { return m_label; } }

    public string GetDisplayText() {  return m_displayText; }
    public bool IsDebuff() {  return m_isDebuff; }  
    public Buff(BuffType type, float value, int turns, string buffLabel)
    {
      m_type = type;
      m_value = value;
      m_turns = turns + 1;
      m_label = buffLabel;

      switch (type)
      {
        case BuffType.INCREASE_ATK_DEALT:
          m_displayText = "+" + value + "Attack for " + turns + " turns";
          m_isDebuff = false;
          break;
        case BuffType.MULTIPLY_ATK_DEALT:
          m_displayText = "x" + value + "Attack for " + turns + " turns";
          m_isDebuff = false;
          break;
        case BuffType.IMMUNITY:
          m_displayText = "No Damage Taken for " + turns + " turns";
          m_isDebuff = false;
          break;
        case BuffType.REDUCE_ATK_DEALT:
          m_displayText = "-" + value + "Attack for " + turns + " turns";
          m_isDebuff = true;
          break;
        case BuffType.REDUCE_SHIELD:
          m_displayText = "-" + value + "Block for " + turns + " turns";
          m_isDebuff = true;
          break;
        case BuffType.SKIP_TURN:
          m_displayText = "Turn skipped";
          m_isDebuff = true;
          break;
        case BuffType.BLEED:
          m_displayText = "Bleeding for " + turns + " turns";
          m_isDebuff = true;
          break;
      }
    }
  }

  public class BuffManager
  {    
    private List<Buff> m_buffs;
    public List<Buff> Buffs { get { return m_buffs; } }

    List<uint> m_buffIcons;

    public int buffsUI;
    readonly float m_buffSize = 50.0f; // Scale of buff icons
    readonly float m_buffPadding = 10.0f;
    uint m_maxNumOfBuffs; // This tracks the highest number of buffs ever

    public BuffManager(int displayID)
    {
      m_buffs = new List<Buff>();
      buffsUI = displayID;
      m_maxNumOfBuffs = 0;
      m_buffIcons= new List<uint>();
    }

    public void AddBuff(Buff buff)
    {
      m_buffs.Add(buff);

      if (m_buffs.Count > m_maxNumOfBuffs)
      {
        m_maxNumOfBuffs = (uint)m_buffs.Count;
        m_buffIcons.Add(Utils.SpawnPrefab(buff.Label));
      }
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

      for (int i = 0; i < m_buffIcons.Count; ++i)
      {
        Utils.SetPosition(m_buffIcons[i], new Vec3<double>(i * (m_buffSize + m_buffPadding), 0.0, 0.0));
      }
    }
  }
}
