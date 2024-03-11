using GoopScripts.Cards;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms.Design;

namespace GoopScripts.Source.Gameplay
{
  public class AnimationManager
  {
    public string m_idle, m_flinch, m_block, m_death, m_deathShield;
    Dictionary<CardBase.CardID, string> m_animations;
    
    double m_timer, m_timeToHit;
    public uint m_statsEntity;
    bool m_isPlaying, m_backToIdle;

    public AnimationManager()
    {
      m_timer = m_timeToHit = 0.0;
      m_isPlaying = m_backToIdle = false;
      m_animations = new Dictionary<CardBase.CardID, string>();
    }

    public void LoadAnimations(Dictionary<CardBase.CardID, string> data)
    {
      m_animations = data;
      foreach (var entry in data)
      {
        Console.WriteLine($"Key: {entry.Key}, Value: {entry.Value}");
      }

    }

    public void AddAnimation(CardBase.CardID card, string anim)
    {
      m_animations.Add(card, anim);
    }

    public void PlayAnimation(CardBase.CardID card, bool backToIdle = true)
    {
      string animToPlay = m_animations[card];
      Console.WriteLine("Playing " + animToPlay + " on " + m_statsEntity);
      Utils.PlayAnimation(animToPlay, m_statsEntity);
      Console.WriteLine("Anim time: " + Utils.GetAnimationTime(animToPlay));
      m_timeToHit = Utils.GetAnimationTime(animToPlay);
      m_backToIdle = backToIdle;
      m_isPlaying = true;
    }

    public void PlayFlinch()
    {
      Console.WriteLine("Playing " + m_flinch);
      Utils.PlayAnimation(m_flinch, m_statsEntity);
      m_timeToHit = Utils.GetAnimationTime(m_flinch);
      m_isPlaying = m_backToIdle = true;
    }

    public void PlayBlock()
    {
      Console.WriteLine("Playing " + m_block);
      Utils.PlayAnimation(m_block, m_statsEntity);
      m_timeToHit = Utils.GetAnimationTime(m_block);
      m_isPlaying = m_backToIdle = true;
    }

    public void PlayDeath()
    {
      Console.WriteLine("Playing " + m_death);
      Utils.PlayAnimation(m_death, (uint)m_statsEntity);
      m_timeToHit = Utils.GetAnimationTime(m_death);
      m_isPlaying = true;
      m_backToIdle = false;
    }

    public void PlayShieldDeath()
    {
      Console.WriteLine("Playing " + m_deathShield);
      Utils.PlayAnimation(m_deathShield, (uint)m_statsEntity);
      m_timeToHit = Utils.GetAnimationTime(m_deathShield);
      m_isPlaying = true;
      m_backToIdle = false;
    }

    public void Update(double dt)
    {
      if (!m_isPlaying)
      {
        return;
      }
       
      m_timer += dt;

      if (m_timer >= m_timeToHit)
      {
        m_timer = 0.0;
        m_isPlaying = false;
        if (m_backToIdle)
        {
          Utils.PlayAnimation(m_idle, (uint)m_statsEntity);
        }
      }
    }

    public string GetAnimation(CardBase.CardID card)
    {
      return m_animations[card];
    }

    public bool IsPlayingAnimation()
    {
      return m_isPlaying;
    }
  }
}
