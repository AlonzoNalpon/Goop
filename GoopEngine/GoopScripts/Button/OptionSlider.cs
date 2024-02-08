using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class OptionSlider : IButtonClick
  {

    // public OptionSlider(uint entity) :base (entity) { }
    bool init = false;
    uint entityID;
    bool m_slide = false;
    double m_posY = 0;
    double m_posZ = 0;
    double m_mousePosX = 0;
    double m_minX = 0;
    double m_maxX = 0;
    public double Range;

    public void OnCreate()
    {
      
    }

    public void OnClick(uint entity)
    {
      if (!init)
      {
        entityID = entity;
        m_posY = Utils.GetPosition(entityID).Y;
        m_posZ = Utils.GetPosition(entityID).Z;
        m_minX = Utils.GetPosition(entityID).X - Range;
        m_maxX = Utils.GetPosition(entityID).X + Range;
        init = true;
      }
      m_slide = true;
      // Console.Write("Entity: " + entityID + " m_slide is enabled.\n");
    }

    public void OnUpdate(double deltaTime)
    {
      if (Utils.IsKeyReleased(Input.KeyCode.MOUSE_BUTTON_1))
      {
        m_slide = false;
        // Console.Write("m_slide is disabled.\n");
      }

      if (m_slide)
      {
        m_mousePosX = Utils.GetMousePosWorld().X;
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
          Vec3<double> newPos = new Vec3<double>(m_mousePosX, m_posY , m_posZ);
          Utils.SetPosition(entityID, newPos);
        }
      }
    }
  }
}
