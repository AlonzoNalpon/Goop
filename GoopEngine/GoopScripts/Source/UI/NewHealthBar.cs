/*!*********************************************************************
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

namespace GoopScripts.UI
{
  public class NewHealthBar : Entity
  {
    public int m_healthBarUI;
    public int m_isPlayer;
    public int m_health;
    public int m_maxHealth;
    uint m_playerBarID;
    private Vec3<double> m_alignPos = new Vec3<double>(0, 0, 0); // starting position
    int m_maxWidth; // taken from the sprite of playerbarID
    int m_oneUnit;

    public NewHealthBar(uint entity) : base(entity)
    {
    }

    public void OnCreate()
    {
      // starting position, move 1/2 width x wise
      // get max width and make it equivalent to max hp
      // set width of the health bar a ratio of current hp vs max hp
      // align it to the left/right depending on character

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
      Utils.SetObjectHeight(m_playerBarID, (int)Utils.GetObjectHeight((uint)m_healthBarUI)* (int)Utils.GetScale((uint)m_healthBarUI).Y);
      UpdateBar();
    }

    public void UpdateBar()
    {
      int newWidth = m_oneUnit * m_health;
      Utils.SetObjectWidth(m_playerBarID, newWidth);
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
  }
}
