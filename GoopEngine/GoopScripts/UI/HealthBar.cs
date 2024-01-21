using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Permissions;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.UI
{
  public class HealthBar : MonoBehaviour
  {
    static private readonly int PADDING_SIZE = 5;
    private int health;
    private int m_maxHealth;
    public int ID = 28;
    private int m_width, m_height;
    private Vec3<double> m_barPos;
    private int m_individualBarWidth;
    uint[] m_bars;

    public void OnCreate()
    {

      m_barPos = Utils.GetPosition((uint)ID);
      m_width = (int)Utils.GetObjectWidth((uint)ID) * (int)Utils.GetScale((uint)ID).X;
      m_height = (int)Utils.GetObjectHeight((uint)ID) * (int)Utils.GetScale((uint)ID).Y - PADDING_SIZE;
      m_maxHealth = health = ((Stats)Utils.GetScript("Player", "Stats")).m_health;

      m_bars = new uint[m_maxHealth];

      m_individualBarWidth = ((m_width - PADDING_SIZE) / m_maxHealth) - PADDING_SIZE;
      Vec3<double> currentBarPos = new Vec3<double>(m_barPos.X - (m_width * 0.5) + PADDING_SIZE + (m_individualBarWidth * 0.5), m_barPos.Y, m_barPos.Z + 0.1);
      Console.WriteLine("Bar Position: " + m_barPos.X + "|" + m_barPos.Y);
      Console.WriteLine("Bar Size: " + m_width + "|" + m_height);

      for (int i = 0; i < m_maxHealth; i++)
      {
        uint barID = Utils.CreateObject("TestHealthBar", currentBarPos, new Vec3<double>((double)m_individualBarWidth, (double)m_height, 1), new Vec3<double>(), (uint)ID);
        Utils.UpdateSprite(barID, "Red");
        m_bars[i] = barID;
        currentBarPos.X += m_individualBarWidth + PADDING_SIZE;
      }

    }

    public void DecreaseHealth(int amount)
    {
      for (int i = 0;i < amount;i++)
      {
        if (health - 1 < 0)
          break;
        Utils.SetIsActiveEntity(m_bars[--health], false);
        Console.WriteLine("Health Decreased");

      }
    }

    public void IncreaseHealth(int amount)
    {
      for (int i = 0; i < amount; i++)
      {
        if (health + 1 > m_maxHealth)
          break;
        Utils.SetIsActiveEntity(m_bars[health++], true);
        Console.WriteLine("Health Increased");

      }
    }
  }
}
