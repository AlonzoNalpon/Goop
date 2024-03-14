/*!*********************************************************************
\file   CharacterTypes.cs
\author c.phua\@digipen.edu
\date   15-January-2024
\brief  Definition of the Combo class and its 2 derived classes
        DrawCombo and BuffCombo. These are used to apply the respective
        combo effects when a combo is triggered. Handled by combo
        manager.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace GoopScripts.Gameplay
{
  public abstract class Combo
  {
    string m_name;
    public Combo(string name)
    {
      m_name = name;
    }
    /*!*********************************************************************
		\brief
		  Function all combos should define. Based on the type of combo,
      different effects will be applied to either source, target or
      both.
		************************************************************************/
    public abstract bool ApplyEffect(ref Stats source, ref Stats target);

    public abstract bool FakeApplyEffect(ref Stats source, ref Stats target);

    /*!*********************************************************************
		\brief
		  Retrieves the name of the current combo
    \return
      The name of the combo
		************************************************************************/
    public string GetName()
    {
      return m_name;
    }
  }

  public class BuffCombo : Combo
  {

    Random m_rand = new Random();
    List<Buff> m_effects;
    int m_chance; // chance out of 100

    public BuffCombo(string name, Buff buff, int chance = 100) : base(name)
    {
      m_effects = new List<Buff> { buff };
      m_chance = chance;
    }

    public BuffCombo(string name, List<Buff> effects, int chance = 100) : base(name)
    {
      m_effects = effects;
      m_chance= chance;
    }

    override public bool ApplyEffect(ref Stats source, ref Stats target)
    {
      if (m_chance <= 100 && !(m_rand.Next(0, 100) < m_chance))
      {
        return false;
      }

      foreach (Buff b in m_effects)
      {
        if (b.IsDebuff())
        {
          target.m_buffs.AddBuff(b);
        }
        else
        {
          source.m_buffs.AddBuff(b);
        }
      }

      return true;
    }


    override public bool FakeApplyEffect(ref Stats source, ref Stats target)
    {
      if (m_chance <= 100 && !(m_rand.Next(0, 100) < m_chance))
      {
        return false;
      }

      foreach (Buff b in m_effects)
      {
        Buff newBuff = new Buff(b);
        if (b.IsDebuff())
        {
          target.m_buffs.Buffs.Add(newBuff); ;
        }
        else
        {
          source.m_buffs.Buffs.Add(newBuff); ;
        }
      }
      return true;
    }
  }

  public class DrawCombo : Combo
  {
    public DrawCombo(string name) : base(name) { }
    override public bool ApplyEffect(ref Stats source, ref Stats target)
    {
      source.Draw();
      return true;
    }

    override public bool FakeApplyEffect(ref Stats source, ref Stats target)
    {
      source.FakeDraw();
      return true;
    }
  }
}
