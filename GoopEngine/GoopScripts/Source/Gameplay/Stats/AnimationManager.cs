/*!*********************************************************************
\file   AnimationManager.cs
\author chengen.lau\@digipen.edu
\date   15-March-2024
\brief  Class that handles playing the animation for a character.
        It stores an internal mapping of each card to animation, as
        well as compute the time it takes for that animation to
        play. Upon playing an animation via function call, a timer is
        elapsed and IsPlayingAnimation() can be used to check whether
        the animation has ended. 
        The class also holds a string of preset actions every character
        has to store the relevant animation names.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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

    /*!*********************************************************************
    \brief
      Stores the mappings of CardID to animation given the dictionary
    \param data
      The dictionary data
    ************************************************************************/
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

    /*!*********************************************************************
    \brief
      Adds a new animation into the AnimationManager
    \param card
      The key (CardID)
    \param data
      The value (animation name)
    ************************************************************************/
    public void AddAnimation(CardBase.CardID card, string anim)
    {
      m_cardAnims.Add(card, new Tuple<string, double>(anim, Utils.GetAnimationTime(anim)));
    }

    /*!*********************************************************************
    \brief
      Plays the animation mapped to the given CardID
    \param card
      The CardID of the card
    \param backToIdle
      Whether to return to idle state after the animation
    ************************************************************************/
    public void PlayAnimation(CardBase.CardID card, bool backToIdle = true)
    {
      var animToPlay = m_cardAnims[card];
      Utils.PlayAnimation(animToPlay.Item1, m_statsEntity);
      m_timeToHit = animToPlay.Item2;
      AnimationPlayed(backToIdle);
    }

    /*!*********************************************************************
    \brief
      Plays the animation for the flinch action. The animation does
    not return to idle state.
    ************************************************************************/
    public void PlayFlinch()
    {
      Utils.SendString("Played " + m_flinch);
      Utils.PlayAnimation(m_flinch, m_statsEntity);
      m_timeToHit = m_characterAnimTime[m_flinch];
      AnimationPlayed(true);
    }

    /*!*********************************************************************
    \brief
      Plays the animation for the block action. The animation does not 
    return to idle state.
    ************************************************************************/
    public void PlayBlock()
    {
      Utils.PlayAnimation(m_block, m_statsEntity);
      m_timeToHit = m_characterAnimTime[m_block];
      AnimationPlayed(true);
    }

    /*!*********************************************************************
    \brief
      Plays the animation for the death action. The animation does not 
      return to idle state.
    ************************************************************************/
    public void PlayDeath()
    {
      Utils.PlayAnimation(m_death, (uint)m_statsEntity);
      m_timeToHit = m_characterAnimTime[m_death];
      AnimationPlayed(false);
    }

    /*!*********************************************************************
    \brief
      Plays the animation for the death action (with shield). The animation 
      does not return to idle state.
    ************************************************************************/
    public void PlayShieldDeath()
    {
      Utils.PlayAnimation(m_deathShield, (uint)m_statsEntity);
      m_timeToHit = m_characterAnimTime[m_deathShield];
      AnimationPlayed(false);
    }

    /*!*********************************************************************
    \brief
      Updates the internal timer usign the deltaTime from the main update
      loop.
    ************************************************************************/
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

    /*!*********************************************************************
    \brief
      Gets the animation name mapped to the given CardID
    \return
      The animation name
    ************************************************************************/
    public string GetAnimation(CardBase.CardID card)
    {
      return m_cardAnims[card].Item1;
    }

    /*!*********************************************************************
    \brief
      Checks whether an animation is currently being played
    \return
      True if an animation is still playing and false otherwise
    ************************************************************************/
    public bool IsPlayingAnimation()
    {
      return m_isPlaying;
    }

    /*!*********************************************************************
    \brief
      Helper function to revert internal variables back to default after
      an animation is played
    ************************************************************************/
    void AnimationPlayed(bool backToIdle)
    {
      m_isPlaying = true;
      m_backToIdle = backToIdle;
      m_timer = 0.0;
    }
  }
}
