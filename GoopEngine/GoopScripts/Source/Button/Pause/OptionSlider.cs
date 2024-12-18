﻿/*!*********************************************************************
\file   OptionSlider.cs
\author loh.j\@digipen.edu
\date   08-February-2024
\brief
  Script used link the visual option slider to the actual volume.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class OptionSlider : Entity, IButtonClick
  {
    bool m_slide = false;
    double m_posY = 0;
    double m_posZ = 0;
    double m_mousePosX = 0;
    double m_minX = 0;
    double m_maxX = 0;
    public double Range;
    public int Index; // 0 = Sound, 1 = Music, 2 = SFX, 3 = Voice
    float volume = 0.0f;
    Utils.ChannelType channel;

    public OptionSlider(uint entity) : base (entity) { }

    /*!*********************************************************************
    \brief
      OnCreate function to initialize variables for the class
    ************************************************************************/
    public void OnCreate()
    {
      Vec3<double> pos = Utils.GetPosition(entityID);
      m_posY = pos.Y;
      m_posZ = pos.Z;
      m_minX = pos.X - Range;
      m_maxX = pos.X + Range;
      switch (Index)
      {
        case 0:
          channel = Utils.ChannelType.BGM;
          break;
        case 1:
          channel = Utils.ChannelType.SFX;
          break;
        case 2:
          channel = Utils.ChannelType.VOICE;
          break;
        case 3:
          break;
        default:
          break;
      }
    }

    /*!*********************************************************************
    \brief
      Sets the sliding flag to true upon click detection
    \param entity
      The entity ID of the current object
    ************************************************************************/
    public void OnClick(uint entity)
    {
      m_slide = true;
    }

    /*!*********************************************************************
    \brief
      Helper function to retrieve the current X position of the object
    \return
      The X position of the object
    ************************************************************************/
    float GetFloatPos()
    {
      double currentPos = Utils.GetPosition(entityID).X;
      return (float)((currentPos - m_minX) / (Range * 2));
    }

    /*!*********************************************************************
    \brief
      Sets the volume of the relevant channels based on where the player
      drags the sliders to
    \param deltaTime
      The delta time of the current frame
    ************************************************************************/
    public void OnUpdate(double deltaTime)
    {
      if (Index == 3 && Utils.GetMasterVolume() != volume)
      {
        volume = Utils.GetMasterVolume();
        Utils.SetPosition(entityID, new Vec3<double>(Range * 2 * volume + m_minX, m_posY, m_posZ));
      }

      if (Utils.GetChannelVolume(channel) != volume && Index != 3)
      {
        volume = Utils.GetChannelVolume(channel);
        Utils.SetPosition(entityID, new Vec3<double>(Range * 2 * volume + m_minX, m_posY, m_posZ));
      }

      if (!m_slide)
      {
        return;
      }


      if (Utils.IsKeyPressed(Input.KeyCode.MOUSE_BUTTON_1))
      {
        m_mousePosX = Utils.GetMousePosWorld().X;
        if (Index == 3)
        {
          Utils.SetMasterVolume(GetFloatPos());
        }
        else
        {
          Utils.SetChannelVolume(channel, GetFloatPos());
        }
        if (m_mousePosX < m_minX)
        {
          Utils.SetPosition(entityID, new Vec3<double>(m_minX, m_posY, m_posZ));
        }
        else if (m_mousePosX > m_maxX)
        {
          Utils.SetPosition(entityID, new Vec3<double>(m_maxX, m_posY, m_posZ));
        }
        else
        {
          Vec3<double> newPos = new Vec3<double>(m_mousePosX, m_posY, m_posZ);
          Utils.SetPosition(entityID, newPos);
        }
      }
      else
      {
        m_slide = false;
      }
    }
  }
}
