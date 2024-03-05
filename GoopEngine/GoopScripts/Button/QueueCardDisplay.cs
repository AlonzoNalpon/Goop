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

    static public bool m_cardSelected;
    static bool m_isHovering = false;
    static uint m_cardInstance;

    public static void ShowCard(uint entity)
    {
      if (m_isHovering)
      {
        return;
      }

      string prefabName = Utils.GetEntityName(Utils.GetParentEntity(entity)) + "_Hover";
      Vec3<double> pos = Utils.GetWorldPosition(entity);
      pos.X += X_OFFSET;
      m_cardInstance = Utils.SpawnPrefab(prefabName, pos);
      m_isHovering = true;
    }

    public static void ShowEnemyCard(uint entity)
    {
      if (m_isHovering) return;

      string prefabName = Utils.GetEntityName(entity);
      Vec3<double> pos = Utils.GetWorldPosition(entity);
      pos.X -=  X_OFFSET;
      m_cardInstance = Utils.SpawnPrefab(prefabName, pos);
      m_isHovering = true;
    }

    public static void DestroyCard()
    {
      if (!m_isHovering)
      {
        return;
      }

      Utils.DestroyEntity(m_cardInstance);
      m_isHovering = false;
    }
  }
}
