/*!*********************************************************************
\file   QueueCardDisplay.cs
\author chengen.lau\@digipen.edu
\date   15-March-2024
\brief  This script handles the displaying of the full card sprite when
        an icon is hovered on by spawning a prefab.

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
  public static class QueueCardDisplay
  {
    static readonly double X_OFFSET = 200.0;

    static bool m_isHovering = false;
    static string m_cardInstance;

    /*!*********************************************************************
    \brief
      Spawns the card hover prefab for the player's queue
    \param deltaTime
      The card icon (in queue) entity
    ************************************************************************/
    public static void ShowCard(uint entity)
    {
      if (m_isHovering)
      {
        return;
      }

      m_cardInstance = Utils.GetEntityName(Utils.GetParentEntity(entity)) + "_Hover";
      Vec3<double> pos = Utils.GetWorldPosition(entity);
      pos.X += X_OFFSET;
      Utils.SpawnPrefab(m_cardInstance, pos);
      m_isHovering = true;
    }

    /*!*********************************************************************
    \brief
      Spawns the card hover prefab for the enemy's queue
    \param deltaTime
      The card icon (in queue) entity
    ************************************************************************/
    public static void ShowEnemyCard(uint entity)
    {
      if (m_isHovering) return;

      m_cardInstance = Utils.GetEntityName(entity) + "_Hover";
      Vec3<double> pos = Utils.GetWorldPosition(entity);
      pos.X -=  X_OFFSET;
      Utils.SpawnPrefab(m_cardInstance, pos);
      m_isHovering = true;
    }

    /*!*********************************************************************
    \brief
      Destroys the card hover prefab instance. This should be called when
      the icon is no longer hovered on
    ************************************************************************/
    public static void DestroyCard()
    {
      if (!m_isHovering)
      {
        return;
      }

      Utils.DestroyEntityByName(m_cardInstance);
      m_isHovering = false;
    }
  }
}
