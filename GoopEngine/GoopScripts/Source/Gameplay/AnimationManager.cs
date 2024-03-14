using GoopScripts.Cards;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms.Design;

namespace GoopScripts.Source.Gameplay
{
  public class AnimationManager
  {
    public string m_idle, m_flinch, m_block, m_death, m_deathShield;
    Dictionary<CardBase.CardID, Tuple<string, double>> m_cardAnims;
    Dictionary<string, double> m_characterAnimTime;
    
    double m_timer, m_timeToHit;
    public uint m_statsEntity;
    bool m_isPlaying, m_backToIdle;

    public AnimationManager()
    {
      m_timer = m_timeToHit = 0.0;
      m_isPlaying = m_backToIdle = false;
      m_cardAnims = new Dictionary<CardBase.CardID, Tuple<string, double>>();
      m_characterAnimTime = new Dictionary<string, double>();
    }

    public void LoadAnimations(Dictionary<CardBase.CardID, string> data)
    {
      // for each card-to-animation mapping,
      // get the animation time and store in container
      foreach (var elem in data)
      {
        AddAnimation(elem.Key, elem.Value);
      }

      // do the same for character anims
      m_characterAnimTime.Add(m_idle, Utils.GetAnimationTime(m_idle));
      m_characterAnimTime.Add(m_flinch, Utils.GetAnimationTime(m_flinch));
      m_characterAnimTime.Add(m_block, Utils.GetAnimationTime(m_block));
      m_characterAnimTime.Add(m_death, Utils.GetAnimationTime(m_death));
      if (!m_characterAnimTime.ContainsKey(m_deathShield))
      {
        m_characterAnimTime.Add(m_deathShield, Utils.GetAnimationTime(m_deathShield));
      }
    }

    public void AddAnimation(CardBase.CardID card, string anim)
    {
      m_cardAnims.Add(card, new Tuple<string, double>(anim, Utils.GetAnimationTime(anim)));
    }

    public void PlayAnimation(CardBase.CardID card, bool backToIdle = true)
    {
      var animToPlay = m_cardAnims[card];
      Utils.PlayAnimation(animToPlay.Item1, m_statsEntity);
      m_timeToHit = animToPlay.Item2;
      AnimationPlayed(backToIdle);
    }

    public void PlayFlinch()
    {
      Utils.PlayAnimation(m_flinch, m_statsEntity);
      m_timeToHit = m_characterAnimTime[m_flinch];
      AnimationPlayed(true);
    }

    public void PlayBlock()
    {
      Utils.PlayAnimation(m_block, m_statsEntity);
      m_timeToHit = m_characterAnimTime[m_block];
      AnimationPlayed(true);
    }

    public void PlayDeath()
    {
      Utils.PlayAnimation(m_death, (uint)m_statsEntity);
      m_timeToHit = m_characterAnimTime[m_death];
      AnimationPlayed(false);
    }

    public void PlayShieldDeath()
    {
      Utils.PlayAnimation(m_deathShield, (uint)m_statsEntity);
      m_timeToHit = m_characterAnimTime[m_deathShield];
      AnimationPlayed(false);
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
      return m_cardAnims[card].Item1;
    }

    public bool IsPlayingAnimation()
    {
      return m_isPlaying;
    }

    void AnimationPlayed(bool backToIdle)
    {
      m_isPlaying = true;
      m_backToIdle = backToIdle;
      m_timer = 0.0;
    }
  }
}
