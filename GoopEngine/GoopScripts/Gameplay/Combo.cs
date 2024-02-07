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
    public abstract void ApplyEffect(ref Stats source, ref Stats target);

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

    override public void ApplyEffect(ref Stats source, ref Stats target)
    {
      foreach (Buff b in m_effects)
      {
        if (m_chance <= 100 && !(m_rand.Next(0, 100) < m_chance))
        {
          continue;
        }

        if (b.IsDebuff())
        {
          target.m_buffs.AddBuff(b);
        }
        else
        {
          source.m_buffs.AddBuff(b);
        }
      }
    }
  }

  public class DrawCombo : Combo
  {
    public DrawCombo(string name) : base(name) { }
    override public void ApplyEffect(ref Stats source, ref Stats target)
    {
      source.Draw();
    }
  }
}
