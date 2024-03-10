﻿/*!*********************************************************************
\file   NewHealthBar.cs
\author loh.j\@digipen.edu
\date   09 March 2024
\brief
  New script used to generate healthbar UI for both player and enemy.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;

namespace GoopScripts.UI
{
  public class NewHealthBar : Entity
  {
    public int m_isPlayer;
    public int m_healthBarUI;
    public int textUI;
    public int m_health;
    public int m_maxHealth;
    uint m_playerBarID;
    private Vec3<double> m_alignPos = new Vec3<double>(0, 0, 0); // starting position
    int m_maxWidth; // taken from the sprite of playerbarID
    int m_oneUnit;

    public NewHealthBar(uint entity) : base(entity)
    {
    }

    public void Init(int health, int maxHealth)
    {
      m_health = health;
      m_maxHealth = maxHealth;

      m_maxWidth = (int)Utils.GetObjectWidth((uint)m_healthBarUI);
      m_alignPos = Utils.GetPosition((uint)m_healthBarUI);
      m_oneUnit = m_maxWidth / m_maxHealth;
      if (m_health < m_maxHealth)
      {
        if (m_isPlayer == 1)
        {
          m_alignPos.X = m_alignPos.X - m_oneUnit * 0.5f * (m_maxHealth - m_health);
        }
        else
        {
          m_alignPos.X = m_alignPos.X + m_oneUnit * 0.5f * (m_maxHealth - m_health);
        }
      }
      m_playerBarID = Utils.CreateObject("TestHealthBar", new Vec3<double>(m_alignPos), new Vec3<double>(1, 1, 1), new Vec3<double>(), (uint)m_healthBarUI);
      if (m_isPlayer == 1)
      {
        Utils.UpdateSprite(m_playerBarID, "UI_Health_Green");
      }
      else
      {
        Utils.UpdateSprite(m_playerBarID, "UI_Health_Red");
      }
      Utils.SetObjectHeight(m_playerBarID, (int)Utils.GetObjectHeight((uint)m_healthBarUI) * (int)Utils.GetScale((uint)m_healthBarUI).Y);
      UpdateBar();
    }

    public void UpdateBar()
    {
      int newWidth = m_oneUnit * m_health;
      Utils.SetObjectWidth(m_playerBarID, newWidth);
      UpdateHealthText();
    }

    public void DecreaseHealth(int amount = 1)
    {
      m_health -= amount;
      UpdateBar();
      Vec3<double> a = Utils.GetPosition(m_playerBarID);
      if (m_isPlayer == 1)
      {
        a.X = a.X - m_oneUnit * 0.5f;
      }
      else
      {
        a.X = a.X + m_oneUnit * 0.5f;
      }
      Utils.SetPosition(m_playerBarID, a);
    }

    public void IncreaseHealth(int amount = 1)
    {
      m_health += amount;
      UpdateBar();
      Vec3<double> a = Utils.GetPosition(m_playerBarID);
      if (m_isPlayer == 1)
      {
        a.X = a.X + m_oneUnit * 0.5f;
      }
      else
      {
        a.X = a.X - m_oneUnit * 0.5f;
      }
      Utils.SetPosition(m_playerBarID, a);
    }

    public void SetHealth(int amount)
    {
      m_health = amount;
    }

    public void SetMaxHealth(int amount)
    {
      m_maxHealth = amount;
    }

    public void OnUpdate(double deltaTime)
    {
      if (Utils.IsKeyTriggered(Input.KeyCode.UP_ARROW))
      {
        IncreaseHealth();
      }
      if (Utils.IsKeyTriggered(Input.KeyCode.DOWN_ARROW))
      {
        DecreaseHealth();
      }
    }

    void UpdateHealthText()
    {
      Utils.SetTextComponent(textUI, m_health + " / " + m_maxHealth);
    }
  }
}
