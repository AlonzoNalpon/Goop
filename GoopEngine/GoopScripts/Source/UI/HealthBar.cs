/*!*********************************************************************
\file       HealthBar.cs
\author     loh.j\@digipen.edu
\co-author  chengen.lau\@digipen.edu
\date       09-March-2024
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
    static readonly double HEAL_ANIM_BUFFER = 0.25;  // buffer time to revert scale of healthbar after the animation
    static readonly Vec3<double> HEALTH_BAR_START = new Vec3<double>(-760.0, 430.0, 3.0);
    static readonly double TIMESLICE_MULTIPLIER = 0.5;

    bool m_isPlayer;
    public int m_healthBarUI;
    int m_textUI;
    public int m_health, m_maxHealth;
    uint m_playerBarID;
    private Vec3<double> m_alignPos = new Vec3<double>(); // starting position
    int m_maxWidth; // taken from the sprite of playerbarID
    int m_oneUnit;

    // animation stuff
    bool m_isPlayingAnim = false, m_isHealing = false;
    double m_timer;
    double m_timeSlice; // how often to update health text
    int m_targetHealth;

    public HealthBar()
    {

    }

    /*!******************************************************************
		\brief
			Initializes the healthbar given the health, max health, entity
      IDs of the UI elements and whether its the player or enemy
		\param health
		  The current health of the character
    \param maxHealth
      The maximum health of the character
    \param isPlayer
      Whether the character is the player
     \param textUI
      The entity ID of the text element
    \param healthBarUI
      The entityID of the health bar element
		********************************************************************/
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
      Utils.SetScript(m_playerBarID, "ResetHealthBar");
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

    /*!******************************************************************
		\brief
			Triggers the
		\param dt
			The delta time of the current frame
		********************************************************************/
    public void Update(double dt)
    {
      if (!m_isPlayingAnim)
      {
        return;
      }

      m_timer += dt;
      if (m_timer >= m_timeSlice / TIMESLICE_MULTIPLIER)
      {
        ++m_health;
        UpdateHealthText();
        m_timer -= m_timeSlice;

        if (m_health >= m_targetHealth)
        {
          m_isPlayingAnim = false;
          Utils.DestroyEntity(Utils.GetEntity(HEAL_EMITTER_PREFAB));
          Utils.DestroyEntity(Utils.GetEntity(HEALTH_BAR_GLOW_PREFAB));
          return;
        }
      }
    }

    /*!******************************************************************
		\brief
			Updates the healthbar by setting it to the correct width based
      on the current health
		********************************************************************/
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

    /*!******************************************************************
		\brief
			Decreases the health in the healthbar
		\param amount
			The amount of health lost
		********************************************************************/
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

    /*!******************************************************************
		\brief
			Increases the health in the healthbar
		\param amount
			The amount of health gained
		********************************************************************/
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

    /*!******************************************************************
		\brief
			Sets the health of the healthbar
		\param amount
			The amount of health to set
		********************************************************************/
    public void SetHealth(int amount)
    {
      m_health = amount;
    }

    /*!******************************************************************
		\brief
			Sets the max health of the healthbar
		\param amount
			The amount of health to set
		********************************************************************/
    public void SetMaxHealth(int amount)
    {
      m_maxHealth = amount;
    }

    /*!******************************************************************
		\brief
			Triggers the heal animation for the player
		\param amount
			The amount of health to heal
    \param time
      The time for the animation
		********************************************************************/
    public void AnimatedHeal(int amount, double time)
    {
      if (amount == 0)
      {
        return;
      }

      m_isHealing = true;
      time -= HEAL_ANIM_BUFFER;
      m_targetHealth = m_health + amount;
      Vec3<double> currPos = Utils.GetPosition(m_playerBarID);
      currPos.X = HEALTH_BAR_START.X + 0.5 * (double)(m_oneUnit * m_health);
      Vec3<double> currScale = new Vec3<double>(1.0, 1.0, 1.0);
      double posIncr = (double)m_oneUnit * 0.5 * TIMESLICE_MULTIPLIER, scaleIncr = TIMESLICE_MULTIPLIER * ((double)m_targetHealth / (double)m_health - 1.0) / (double)amount;
      m_timeSlice = time / (double)amount;

      // spawn emitter prefab and glow
      uint emitterInst = Utils.SpawnPrefab(HEAL_EMITTER_PREFAB);
      Vec3<double> glowPos = new Vec3<double>(HEALTH_BAR_START.X + (double)m_oneUnit * 0.5 * (double)m_targetHealth, HEALTH_BAR_START.Y, currPos.Z - 1);

      uint glowInst = Utils.CreateEntity(HEALTH_BAR_GLOW_PREFAB, glowPos, new Vec3<double>((double)(m_targetHealth + 2) / (double)m_maxHealth, 1.0, 1.0));
      Utils.UpdateSprite(glowInst, "HP_Glow");
      Utils.SetObjectWidth(glowInst, m_maxWidth);

      // set up animation keyframes
      Utils.ClearTweenKeyFrames(m_playerBarID, "Heal");
      amount = (int)((double)amount / TIMESLICE_MULTIPLIER);

      Utils.AddTweenKeyFrame(emitterInst, "Heal", currPos, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), 0.0);
      for (int i = 0; i < amount; ++i)
      {
        currPos.X += posIncr;
        currScale.X += scaleIncr;
        Utils.AddTweenKeyFrame(m_playerBarID, "Heal", currPos, currScale, new Vec3<double>(), m_timeSlice * TIMESLICE_MULTIPLIER, (i % 2 == 0) ? 1.0f : 0.0f);

        Vec3<double> emitterPos = new Vec3<double>(currPos);
        emitterPos.X += (double)m_oneUnit * (double)(i + m_health) * 0.5 - (double)m_oneUnit;
        Utils.AddTweenKeyFrame(emitterInst, "Heal", emitterPos, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), m_timeSlice * TIMESLICE_MULTIPLIER);
      }
      Utils.AddTweenKeyFrame(m_playerBarID, "Heal", currPos, currScale, new Vec3<double>(), HEAL_ANIM_BUFFER);
      Vec3<double> finalPos = new Vec3<double>(HEALTH_BAR_START.X + (double)m_oneUnit * 0.5 * (double)m_targetHealth, currPos.Y, currPos.Z);
      Utils.AddTweenKeyFrame(m_playerBarID, "Heal", finalPos, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), 0.0, 1.0f, "ResetHealthBar");

      Utils.PlayTransformAnimation(m_playerBarID, "Heal");
      Utils.PlayTransformAnimation(emitterInst, "Heal");
      Utils.PlaySoundF("SFX_Heal", 0.25f, Utils.ChannelType.SFX, false);

      m_isPlayingAnim = true;
      m_timer = 0.0;
    }

    /*!******************************************************************
		\brief
			Updates the text to match the current health
		********************************************************************/
    void UpdateHealthText()
    {
      Utils.SendString("Updating text of entity " + m_textUI);
      Utils.SetTextComponent(m_textUI, m_health + " / " + m_maxHealth);
    }

    /*!******************************************************************
		\brief
			Resets the health bar to default scale and width after the
      animation
		********************************************************************/
    public void Reset()
    {
      UpdateBar();
      m_isHealing = false;
    }

    /*!******************************************************************
		\brief
			Checks if the heal animation is currently playing
		\return
      True if the animation is ongoing and false otherwise
		********************************************************************/
    public bool IsHealing()
    {
      return m_isHealing;
    }
  }
}
