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
    private int padding_size = 0;
    public int m_health, m_maxHealth; //
    public int healthBarUI; //
    public int Player;
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
      m_height = (int)Utils.GetObjectHeight((uint)healthBarUI) * (int)Utils.GetScale((uint)healthBarUI).Y - padding_size;
      m_bars = new uint[m_maxHealth];

      m_individualBarWidth = ((m_width - padding_size) / m_maxHealth) - padding_size;
      Vec3<double> currentBarPos = new Vec3<double>(m_barPos.X - (m_width * 0.5) + padding_size + (m_individualBarWidth * 0.5), m_barPos.Y, m_barPos.Z + 5.0);

      for (int i = 0; i < m_maxHealth; i++)
      {
        
        uint barID = Utils.CreateObject("TestHealthBar", currentBarPos, new Vec3<double>(1, 1, 1), new Vec3<double>(), (uint)healthBarUI);
        if (Player == 0)
        {
          Utils.UpdateSprite(barID, "UI_HealthBar_Single_Green");
        }
        else
        {
          Utils.UpdateSprite(barID, "UI_HealthBar_Single_Red");
        }
        Utils.SetObjectWidth(barID, m_individualBarWidth);
        Utils.SetObjectHeight(barID, m_height);
        m_bars[i] = barID;
        currentBarPos.X += m_individualBarWidth + padding_size;
      }
    }

    public void ResetBar()
    {
      if (Player != 0)
      {
        Vec3<double> currentScale = Utils.GetScale((uint)healthBarUI);
        Utils.SetScale((uint)healthBarUI, new Vec3<double>(-currentScale.X, currentScale.Y, currentScale.Z));
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
