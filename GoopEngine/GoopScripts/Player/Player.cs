/*!*********************************************************************
\file   Player.cs
\author han.q\@digipen.edu
\date   28 September 2023
\brief
	This c# script will contain the code for player's game logic


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Input;
using GoopScripts.Mono;
using GoopScripts.Component;
using System.Runtime.InteropServices;
using static GoopScripts.Mono.Utils;


namespace GoopScripts.Player
{
  public  class Player : MonoBehaviour
  {

    public int intTest= 32;
    //public float fTest = 32;
    //public double DTest = 32;
    //public Vec2<double> DVec2Test = new Vec2<double>(100.0, 200.0);
    //public Vec3<double> DVec3Test = new Vec3<double>(100.0, 200.0, 300.0);
    public List<int> ListIntTest = new List<int>(10) {1,2,3,4,5,6,7,8,9,10 };

    /*!*********************************************************************
   \brief
     Non default constructor of Player class

   \params enityID
    ID of the owner of this scipt
   ************************************************************************/
    public Player(uint entityID) : base(entityID)
    {
      Console.WriteLine("size of Array: " + Marshal.SizeOf(ListIntTest[0]) * ListIntTest.Count());
    }


    /*!*********************************************************************
   \brief
     Awake function for the player script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the player script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the player script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void OnUpdate(double dt)
    {
      //Console.WriteLine("WHAT\n");
      Transform newChange = new Transform(); // All values are set to 0

      //Console.WriteLine("Dvec3Test val: " + DVec3Test.X + "," + DVec3Test.Y + "," + DVec3Test.Z);
      //Movement logic
      if (IsKeyPressed(KeyCode.W))
      {
        newChange.Pos = new Vec3<double>(newChange.Pos.X, 200.0,0.0) ;

      }
      else if (IsKeyPressed(KeyCode.S))
      {
        newChange.Pos = new Vec3<double>(newChange.Pos.X, -200.0, 0.0);
      }

      if (IsKeyPressed(KeyCode.A))
      {
        newChange.Pos = new Vec3<double>(-200.0, newChange.Pos.Y, 0.0);
      }
      else if (IsKeyPressed(KeyCode.D))
      {
        newChange.Pos = new Vec3<double>(200.0, newChange.Pos.Y, 0.0);
      }

      //Rotation logic
      newChange.Rot = (IsKeyPressed(KeyCode.R)) ? new Vec3<double>(0, 0, -200.0) : (IsKeyPressed(KeyCode.E)) ? new Vec3<double>(0, 0, 200.0) : new Vec3<double>(0, 0, 0);


      newChange.Scale = (IsKeyPressed(KeyCode.T)) ? new Vec3<double>(2, 2,1) : (IsKeyPressed(KeyCode.Y)) ? new Vec3<double>(0.5, 0.5,1) : new Vec3<double>(1.0, 1.0,1);


      if(newChange.Pos.X != 0.0 || newChange.Pos.Y != 0.0)
      {
        SetPosition(base.m_entityID, newChange.Pos);
      }


      if (newChange.Scale.X != 1.0 || newChange.Scale.Y != 0.0)
      {
        SetScale(base.m_entityID, newChange.Scale);
      }


      if (newChange.Rot.Z != 0.0f)
      {
        SetRotation(base.m_entityID, newChange.Rot);
      }
    }

    /*!*********************************************************************
    \brief
     late Update function for the player script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }




  }
}
