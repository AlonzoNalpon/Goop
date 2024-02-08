/*!*********************************************************************
\file   HealthBar.cs
\author loh.j@digipen.edu
\date   08 Febuary 2024
\brief
  Script used to generate healthbar UI for both player and enemy.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
  public class HealthBar : Entity
  {
    static private readonly int PADDING_SIZE = 5;
    public int m_health, m_maxHealth; //
    public int healthBarUI; //
    private int m_width, m_height;
    private Vec3<double> m_barPos;
    private int m_individualBarWidth;
    uint[] m_bars;

    public HealthBar(uint entity) : base(entity)
    {
      
    }

    public void OnCreate()
    {
      Init();
    }

    public void Init()
    {
      m_barPos = Utils.GetPosition((uint)healthBarUI);
      m_width = (int)Utils.GetObjectWidth((uint)healthBarUI) * (int)Utils.GetScale((uint)healthBarUI).X;
      m_height = (int)Utils.GetObjectHeight((uint)healthBarUI) * (int)Utils.GetScale((uint)healthBarUI).Y - PADDING_SIZE;
      m_bars = new uint[m_maxHealth];

      m_individualBarWidth = ((m_width - PADDING_SIZE) / m_maxHealth) - PADDING_SIZE;
      Vec3<double> currentBarPos = new Vec3<double>(m_barPos.X - (m_width * 0.5) + PADDING_SIZE + (m_individualBarWidth * 0.5), m_barPos.Y, m_barPos.Z + 5.0);
      //Console.WriteLine("Bar Position: " + m_barPos.X + "|" + m_barPos.Y);
      //Console.WriteLine("Bar Size: " + m_width + "|" + m_height);

      for (int i = 0; i < m_maxHealth; i++)
      {
        uint barID = Utils.CreateObject("TestHealthBar", currentBarPos, new Vec3<double>((double)m_individualBarWidth, (double)m_height, 1), new Vec3<double>(), (uint)healthBarUI);
        Utils.UpdateSprite(barID, "Red");
        m_bars[i] = barID;
        currentBarPos.X += m_individualBarWidth + PADDING_SIZE;
      }
    }

    public int GetHealth()
    {
      return m_health;
    }

    public void DecreaseHealth(int amount)
    {
      for (int i = 0;i < amount;i++)
      {
        if (m_health - 1 < 0)
          break;
        Utils.SetIsActiveEntity(m_bars[--m_health], false);
        //Console.WriteLine("Health Decreased");

      }
    }

    public void IncreaseHealth(int amount)
    {
      for (int i = 0; i < amount; i++)
      {
        if (m_health + 1 > m_maxHealth)
          break;
        Utils.SetIsActiveEntity(m_bars[m_health++], true);
        //Console.WriteLine("Health Increased");

      }
    }
  }
}
