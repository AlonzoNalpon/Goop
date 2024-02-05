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

    Buff m_effect;
    public BuffCombo(string name, Buff effect) : base(name)
    {
      m_effect = effect;
    }

    override public void ApplyEffect(ref Stats source, ref Stats target)
    {
      source.m_buffs.AddBuff(m_effect);
    }
  }

  public class DebuffCombo : Combo
  {

    Buff m_effect;
    public DebuffCombo(string name, Buff effect) : base(name)
    {
      m_effect = effect;
    }

    override public void ApplyEffect(ref Stats source, ref Stats target)
    {
      target.m_buffs.AddBuff(m_effect);
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
