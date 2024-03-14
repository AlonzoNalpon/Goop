﻿using GoopScripts.Mono;
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

    public static void ShowEnemyCard(uint entity)
    {
      if (m_isHovering) return;

      m_cardInstance = Utils.GetEntityName(entity) + "_Hover";
      Vec3<double> pos = Utils.GetWorldPosition(entity);
      pos.X -=  X_OFFSET;
      Utils.SpawnPrefab(m_cardInstance, pos);
      m_isHovering = true;
    }

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
