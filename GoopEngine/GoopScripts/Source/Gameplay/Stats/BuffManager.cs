/*!*********************************************************************
\file   BuffManager.cs
\author chengen.lau\@digipen.edu
\co-author c.phua\@digipen.edu
\co-author Han Qin Ding
\date   10-January-2024
\brief  
Keeps track of buffs and allows modification to list of buffs for the character.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
      IMMUNE_TO_DAMAGE,

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

    /*!*********************************************************************
    \brief  
      Overloaded constructor of a buff
    \param type
      Type of buff
    \param value
      Value of the buff's effect
    \param turns
      Number of turns the buff lasts
    ************************************************************************/
    public Buff(BuffType type, float value, int turns)
    {
      m_type = type;
      m_value = value;
      m_turns = turns + 1;
      m_isDebuff = type >= BuffType.FLAT_ATK_DOWN;

      switch (type)
      {
        case BuffType.INCREASE_BLOCK:
          m_label = "+" + value + " Block";
          break;
        case BuffType.FLAT_ATK_UP:
          m_label = "+" + value + " Atk";
          break;
        case BuffType.MULTIPLICATIVE_ATK_UP:
        case BuffType.MULTIPLICATIVE_ATK_DOWN:
          m_label = "x" + value + " Atk";
          break;
        case BuffType.FLAT_ATK_DOWN:
          m_label = "-" + value + " Atk";
          break;
        case BuffType.BLIND:
          m_label = "Blinded";
          break;
        case BuffType.SKIP_TURN:
          m_label = "Skipped";
          break;
        case BuffType.IMMUNE_TO_DAMAGE:
          m_label = "Immune To Damage";
          break;
        default:
          m_label = string.Empty;
          break;
      }
    }

    /*!*********************************************************************
    \brief  
      Copy constructor of a buff
    \param rhs
      Buff to copy
    ************************************************************************/
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
    static readonly Dictionary<Buff.BuffType, string> m_buffPrefabs = new Dictionary<Buff.BuffType, string>()
    {
      { Buff.BuffType.INCREASE_BLOCK, "BlockUp" },
      { Buff.BuffType.FLAT_ATK_UP, "DmgUp" },
      { Buff.BuffType.MULTIPLICATIVE_ATK_UP, "AtkUp" },
      { Buff.BuffType.FLAT_ATK_DOWN, "DmgDown" },
      { Buff.BuffType.MULTIPLICATIVE_ATK_DOWN, "DmgDown" },
      { Buff.BuffType.BLIND, "Blinded" },
      { Buff.BuffType.SKIP_TURN, "Skipped" },
      { Buff.BuffType.IMMUNE_TO_DAMAGE, "DamageImmunity" }
    };

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
      uint buffPrefab = Utils.SpawnPrefab(m_buffPrefabs[buff.type]);
      Console.WriteLine("Spawned " + m_buffPrefabs[buff.type] + " with id " + buffPrefab);
      Utils.SetTextComponent((int)Utils.GetChildEntity(buffPrefab, "BuffIconText"), buff.Label);
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


    public void SetBuff(List<Buff> b)
    {
      m_buffs = b;
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

    public void FakeStepTurn()
    {
      foreach (var buff in m_buffs)
      {
        buff.turns -= 1;
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
