﻿
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
      // BUFFS HERE
      INCREASE_BLOCK,
      FLAT_ATK_UP, // e.g. +1
      MULTIPLICATIVE_ATK_UP, // e.g. *2

      // DEBUFFS MUST START HERE
      FLAT_ATK_DOWN, // e.g. -1
      MULTIPLICATIVE_ATK_DOWN, // e.g. *0.5
      BLIND,
      SKIP_TURN,

      TOTAL_BUFFS
    }

    BuffType m_type;
    float m_value;
    string m_label;
    int m_turns;
    bool m_isDebuff;
    public uint m_iconID;

    // Getters
    public BuffType type { get { return m_type; } }
    public float value { get { return m_value; } }
    public int turns { get { return m_turns; } set { m_turns = value; } }

    public bool IsDebuff() { return m_isDebuff; }

    public string Label { get { return m_label; } }

    public Buff(BuffType type, float value, int turns)
    {
      m_type = type;
      m_value = value;
      m_turns = turns + 1;
      m_isDebuff = type >= BuffType.FLAT_ATK_DOWN;

      switch (type)
      {
        case BuffType.INCREASE_BLOCK:
          m_label = "Block Up";
          break;
        case BuffType.FLAT_ATK_UP:
          m_label = "Dmg Up";
          break;
        case BuffType.MULTIPLICATIVE_ATK_UP:
          m_label = "Atk Up";
          break;
        case BuffType.FLAT_ATK_DOWN:
        case BuffType.MULTIPLICATIVE_ATK_DOWN:
          m_label = "Dmg Down";
          break;
        case BuffType.BLIND:
          m_label = "Blinded";
          break;
        case BuffType.SKIP_TURN:
          m_label = "Skipped";
          break;
        default:
          m_label = string.Empty;
          break;
      }
    }

    public Buff(Buff rhs)
    {
      m_type = rhs.m_type;
      m_iconID = rhs.m_iconID;
      m_value = rhs.m_value;
      m_isDebuff = rhs.m_isDebuff;
      m_label = rhs.m_label;
      m_turns = rhs.m_turns;
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
      Buff newBuff = new Buff(buff);
      uint buffPrefab = Utils.SpawnPrefab(buff.Label);
      newBuff.m_iconID = buffPrefab;
      m_buffIcons.Add(buffPrefab);
      m_buffs.Add(newBuff);
      if (m_characterType == CharacterType.PLAYER)
			  Utils.SetParent(Utils.GetEntity("PlayerBuffAnchor"), buffPrefab);
			else
				Utils.SetParent(Utils.GetEntity("EnemyBuffAnchor"), buffPrefab);

      m_maxNumOfBuffs = m_buffs.Count > m_maxNumOfBuffs ? (uint)m_buffs.Count : m_maxNumOfBuffs;
      UpdateBuffsUI();
    }



    public void StepTurn()
		{
      foreach (var buff in m_buffs)
      {
        buff.turns -= 1;
        if (buff.turns <= 0)
        {
          DestroyBuffIcon(buff.m_iconID);
        }
      }
      m_buffs.RemoveAll(buff => buff.turns <= 0);
    }

    public void DestroyBuffIcon(uint iconID)
    {
      Utils.DestroyEntity(iconID);
      m_buffIcons.Remove(iconID);
    }

    public void UpdateBuffsUI()
    {
      int modifier = (m_characterType == CharacterType.PLAYER) ? 1 : -1;
      for (int i = 0; i < m_buffIcons.Count; ++i)
      {
        Utils.SetPosition(m_buffIcons[i], new Vec3<double>(i * modifier * (m_buffSize + m_buffPadding), 0.0, 0.0));
      }
    }
  }
}
