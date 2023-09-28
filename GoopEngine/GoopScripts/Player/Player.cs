using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Input;
using GoopScripts.Mono;


namespace GoopScripts
{
  public  class Player : MonoBehaviour
  {

    public Player(uint entityID) : base(entityID)
    {
      // Derived class-specific initialization can be done here
    }


    public void Awake()
    {
      // Logic for Awake
    }

    public void Start()
    {
      // Logic for Start
    }

    public  void Update()
    {
      Transform newChange = new Transform(); // All values are set to 0
      

      //Movement logic
      if (IsKeyPressed(KeyCode.W))
      {
        newChange.m_pos = new Vec2<double>(newChange.m_pos.x, 200.0) ;

      }
      else if (IsKeyPressed(KeyCode.S))
      {
        newChange.m_pos = new Vec2<double>(newChange.m_pos.x, -200.0);
      }

      if (IsKeyPressed(KeyCode.A))
      {
        newChange.m_pos = new Vec2<double>(-200.0, newChange.m_pos.y);
      }
      else if (IsKeyPressed(KeyCode.D))
      {
        newChange.m_pos = new Vec2<double>(200.0, newChange.m_pos.y);
      }

      //Rotation logic
      newChange.m_rot = (IsKeyPressed(KeyCode.R)) ? -10.0f: (IsKeyPressed(KeyCode.E)) ? 10.0f: 0.0f;


      //Scale Logic
      double scroll = GetMouseScrollY();
      newChange.m_scale = (scroll > 0.0) ? new Vec2<double>(2, 2) : (scroll < 0.0) ? new Vec2<double>(0.5, 0.5) : new Vec2<double>(1.0, 1.0);


      if(newChange.m_pos.x != 0.0 || newChange.m_pos.y != 0.0 || newChange.m_scale.x != 1.0 || newChange.m_scale.y != 1.0 || newChange.m_rot != 0.0f)
      {
        SetTransform(base.m_entityID, newChange);
      }
    }

    public void LateUpdate()
    {
      // Logic for lateUpdate
    }


    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyTriggered(KeyCode k);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyHeld(KeyCode k);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyReleased(KeyCode k);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyPressed(KeyCode k);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static void SetTransform(uint ID,Transform newValue);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static double GetMouseScrollY();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static double GetMouseScrollX();

  }
}
