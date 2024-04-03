/*!*********************************************************************
\file   HealthBar.cs
\author loh.j\@digipen.edu
\date   09 March 2024
\brief
        Script used to generate healthbar UI for both player and enemy.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
  public class HealthBar
  {
    static readonly string HEAL_EMITTER_PREFAB = "HealEmitter";
    static readonly string HEALTH_BAR_GLOW_PREFAB = "HealthBarGlow";

    bool m_isPlayer;
    public int m_healthBarUI;
    int m_textUI;
    public int m_health, m_maxHealth;
    uint m_playerBarID;
    private Vec3<double> m_alignPos = new Vec3<double>(); // starting position
    int m_maxWidth; // taken from the sprite of playerbarID
    int m_oneUnit;

    // animation stuff
    bool m_isPlayingAnim = false;
    double m_timer;
    double m_timeSlice; // how often to update health text
    int m_targetHealth;

    public HealthBar()
    {

    }

    public void Init(int health, int maxHealth, bool isPlayer, int textUI, int healthBarUI)
    {
      m_health = health;
      m_maxHealth = maxHealth;
      m_isPlayer = isPlayer;
      m_textUI = textUI;
      m_healthBarUI = healthBarUI;

      m_maxWidth = (int)Utils.GetObjectWidth((uint)m_healthBarUI) - 10;
      m_alignPos = Utils.GetPosition((uint)m_healthBarUI);
      m_alignPos.Y -= 1.0;
      m_alignPos.Z = 8.0;
      m_oneUnit = m_maxWidth / m_maxHealth;
      if (m_health < m_maxHealth)
      {
        if (m_isPlayer)
        {
          m_alignPos.X = m_alignPos.X - m_oneUnit * 0.5f * (m_maxHealth - m_health);
        }
        else
        {
          m_alignPos.X = m_alignPos.X + m_oneUnit * 0.5f * (m_maxHealth - m_health);
        }
      }
      m_playerBarID = Utils.CreateObject("TestHealthBar", new Vec3<double>(m_alignPos), new Vec3<double>(1, 1, 1), new Vec3<double>());
      Utils.AddTweenComponent(m_playerBarID);
      Utils.SetPosition(m_playerBarID, m_alignPos);

      Utils.SetScale(m_playerBarID, new Vec3<double>(1.0, 1.0, 1.0));
      if (m_isPlayer)
      {
        Utils.UpdateSprite(m_playerBarID, "UI_Health_Green");
      }
      else
      {
        Utils.UpdateSprite(m_playerBarID, "UI_Health_Red");
      }
      Utils.SetObjectHeight(m_playerBarID, (int)Utils.GetObjectHeight((uint)m_healthBarUI) * (int)Utils.GetScale((uint)m_healthBarUI).Y - 18);
      UpdateBar();
    }

    public void Update(double dt)
    {
      if (!m_isPlayingAnim)
      {
        return;
      }

      m_timer += dt;
      if (m_timer >= m_timeSlice)
      {
        ++m_health;
        UpdateHealthText();
        m_timer = 0.0;

        if (m_health >= m_targetHealth)
        {
          UpdateHealthText();
          m_isPlayingAnim = false;
          Utils.DestroyEntity(Utils.GetEntity(HEAL_EMITTER_PREFAB));
          Utils.DestroyEntity(Utils.GetEntity(HEALTH_BAR_GLOW_PREFAB));
        }
      }
    }

    public void UpdateBar()
    {
      int newWidth;
      if (m_health <= 0)
      {
        newWidth = 0;
      }
      else
      {
        newWidth = m_oneUnit * m_health;
      }
      Utils.SetObjectWidth(m_playerBarID, newWidth);
      UpdateHealthText();
    }

    public void DecreaseHealth(int amount = 1)
    {
      m_health -= amount;
      if (m_health < 0)
      {
        m_health = 0;
      }
      UpdateBar();
      Vec3<double> a = Utils.GetPosition(m_playerBarID);
      for (int i = 0; i < amount; ++i)
      {
        if (m_isPlayer)
        {
          a.X = a.X - m_oneUnit * 0.5;
        }
        else
        {
          a.X = a.X + m_oneUnit * 0.5;
        }
      }
      Utils.SetPosition(m_playerBarID, a);
    }

    public void IncreaseHealth(int amount = 1)
    {
      m_health += amount;
      UpdateBar();
      Vec3<double> a = Utils.GetPosition(m_playerBarID);
      for (int i = 0; i < amount; ++i)
      {
        if (m_isPlayer)
        {
          a.X = a.X + m_oneUnit * 0.5;
        }
        else
        {
          a.X = a.X - m_oneUnit * 0.5;
        }
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

    public void AnimatedHeal(int amount, double time)
    {
      m_targetHealth = m_health + amount;
      double multiplier = 0.5;
      Vec3<double> currPos = Utils.GetPosition(m_playerBarID);
      Vec3<double> currScale = new Vec3<double>(1.0, 1.0, 1.0);
      double posIncr = (double)m_oneUnit * 0.5 * multiplier, scaleIncr = multiplier * ((double)m_targetHealth / (double)m_health - 1.0) / (double)amount;
      m_timeSlice = time / (double)amount;

      // spawn emitter prefab and glow
      uint emitterInst = Utils.SpawnPrefab(HEAL_EMITTER_PREFAB);
      Vec3<double> glowPos = new Vec3<double>(currPos.X + ((double)m_oneUnit * 0.5) * (double)amount, currPos.Y, currPos.Z - 1);

      uint glowInst = Utils.CreateEntity(HEALTH_BAR_GLOW_PREFAB, glowPos, new Vec3<double>(1.0, 1.0, 1.0));
      Utils.UpdateSprite(glowInst, "HP_Glow");
      Utils.SetObjectWidth(glowInst, m_oneUnit * (m_targetHealth + 1));

      // set up animation keyframes
      Utils.ClearTweenKeyFrames(m_playerBarID, "Heal");
      amount = (int)((double)amount / multiplier);
      for (int i = 0; i < amount; ++i)
      {
        currPos.X += posIncr;
        currScale.X += scaleIncr;
        Utils.AddTweenKeyFrame(m_playerBarID, "Heal", currPos, currScale, new Vec3<double>(), m_timeSlice * multiplier, (i % 2 == 0 || i == amount - 1) ? 1.0f : 0.0f);

        Vec3<double> emitterPos = new Vec3<double>(currPos);
        emitterPos.X += (double)m_oneUnit * (double)(i + m_health) * 0.5 - (double)m_oneUnit;
        Utils.AddTweenKeyFrame(emitterInst, "Heal", emitterPos, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), m_timeSlice * multiplier, (i % 2 == 0 || i == amount - 1) ? 1.0f : 0.0f);
      }

      Utils.PlayTransformAnimation(m_playerBarID, "Heal");
      Utils.PlayTransformAnimation(emitterInst, "Heal");
      m_isPlayingAnim = true;
      m_timer = 0.0;
    }

    void UpdateHealthText()
    {
      Utils.SetTextComponent(m_textUI, m_health + " / " + m_maxHealth);
    }
  }
}
