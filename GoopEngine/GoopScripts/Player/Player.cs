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
     
     public virtual void Awake()
    {
      // Logic for Awake
    }

    public virtual void Start()
    {
      // Logic for Start
      Vec2<double> tt = GetVec2();
      tt.x = 1001.4;
      tt.y = 2021.123;
      SetVec2(tt);
      //Console.WriteLine("LETSSSSSS FKINGGG GOOOOO\n");
    }

    public virtual void Update()
    {
      // Logic for Update
      if (IsKeyTriggered(KeyCode.A))
      {
        Console.WriteLine("A IS TRIGGERED");
      }
      if (IsKeyHeld(KeyCode.A))
      {
        Console.WriteLine("A IS HELD");
      }
      if (IsKeyReleased(KeyCode.A))
      {
        Console.WriteLine("A IS RELEASED");
      }
    }

    public virtual void LateUpdate()
    {
      // Logic for lateUpdate
    }

    //[MethodImplAttribute(MethodImplOptions.InternalCall)]
    //extern static void CppFunction();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static Vec2<double> GetVec2();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static void SetVec2(Vec2<double> v);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyTriggered(KeyCode k);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyHeld(KeyCode k);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyPressed(KeyCode k);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyReleased(KeyCode k);

  }
}
