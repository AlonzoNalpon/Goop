
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
    //bool m_isDebuff;
    float m_value;
    string m_label;
    int m_turns;
    public uint m_iconID;
    //readonly string m_displayText;

    // Getters
    public BuffType type { get { return m_type; } }
    public float value { get { return m_value; } }
    public int turns { get { return m_turns; } set { m_turns = value; } }

    public string Label { get { return m_label; } }

    //public string GetDisplayText() {  return m_displayText; }
    //public bool IsDebuff() {  return m_isDebuff; }  
    public Buff(BuffType type, float value, int turns, string buffLabel)
    {
      m_type = type;
      m_value = value;
      m_turns = turns + 1;
      m_label = buffLabel;

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

    List<uint> m_buffIcons;

    public int buffsUI;
    readonly float m_buffSize = 50.0f; // Scale of buff icons
    readonly float m_buffPadding = 10.0f;
    uint m_maxNumOfBuffs; // This tracks the highest number of buffs ever
    CharacterType m_characterType;

    public BuffManager(int displayID, CharacterType type)
    {
      m_buffs = new List<Buff>();
      buffsUI = displayID;
      m_maxNumOfBuffs = 0;
      m_buffIcons= new List<uint>();
      m_characterType = type;
    }

    public void AddBuff(Buff buff)
    {
      m_buffs.Add(buff);
			uint buffPrefab = Utils.SpawnPrefab(buff.Label);
			buff.m_iconID = buffPrefab;
			m_buffIcons.Add(buffPrefab);
      if (m_characterType == CharacterType.PLAYER)
			  Utils.SetParent(Utils.GetEntity("PlayerBuffAnchor"), buffPrefab);
			else
				Utils.SetParent(Utils.GetEntity("EnemyBuffAnchor"), buffPrefab);

			m_maxNumOfBuffs = m_buffs.Count > m_maxNumOfBuffs ? (uint)m_buffs.Count : m_maxNumOfBuffs;
    }

    public void StepTurn()
		{
			Console.WriteLine(m_buffs.Count);
			foreach (var buff in m_buffs)
      {
        buff.turns -= 1;
        if (buff.turns <= 0)
        {
          Utils.DestroyEntity(buff.m_iconID);
          m_buffIcons.Remove(buff.m_iconID);
        }
      }
      m_buffs.RemoveAll(buff => buff.turns <= 0);
    }

    public void RemoveBuff()
    {
      Random rng = new Random();
      int chance = rng.Next(0, m_buffs.Count - 1);
			Console.WriteLine($"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!{chance} is being removed by random");
			Utils.DestroyEntity(m_buffIcons.Find(iconid => iconid == m_buffs[chance].m_iconID));
      m_buffs.RemoveAt(chance);
    }

    public void UpdateBuffsUI()
    {
      //string txt = "";
      //foreach (Buff buff in m_buffs)
      //{
      //  txt += buff.GetDisplayText() + "\n";
      //}
      //Utils.SetTextComponent(buffsUI, txt);

      for (int i = 0; i < m_buffIcons.Count; ++i)
      {
        Utils.SetPosition(m_buffIcons[i], new Vec3<double>(i * (m_buffSize + m_buffPadding), 0.0, 0.0));
      }
    }
  }
}
