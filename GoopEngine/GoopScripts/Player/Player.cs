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
      Console.WriteLine("C# ID: " + entityID.ToString());
      // Derived class-specific initialization can be done here
    }


    public virtual void Awake()
    {
      // Logic for Awake
    }

    public virtual void Start()
    {
      // Logic for Start
    }

    public virtual void Update()
    {
      Transform newChange = new Transform(); // All values are set to 0
      // Logic for Update
      if (IsKeyPressed(KeyCode.W))
      {
        newChange.m_pos = new Vec2<double>(newChange.m_pos.x, 5.0) ;

      }
      else if (IsKeyPressed(KeyCode.S))
      {
        newChange.m_pos = new Vec2<double>(newChange.m_pos.x, -5.0);
      }

      if (IsKeyPressed(KeyCode.A))
      {
        newChange.m_pos = new Vec2<double>(-5.0, newChange.m_pos.y);
      }
      else if (IsKeyPressed(KeyCode.D))
      {
        newChange.m_pos = new Vec2<double>(5.0, newChange.m_pos.y);
      }

      if(newChange.m_pos.x != 0.0 || newChange.m_pos.y != 0.0)
      {
        SetTransform(base.m_entityID, newChange);
      }
    }

    public virtual void LateUpdate()
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

  }
}
