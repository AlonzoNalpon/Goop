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


namespace GoopScripts
{
  public  class Player : MonoBehaviour
  {

    /*!*********************************************************************
   \brief
     Non default constructor of Player class

   \params enityID
    ID of the owner of this scipt
   ************************************************************************/
    public Player(uint entityID) : base(entityID)
    {
     
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
    public void Update()
    {
      Transform newChange = new Transform(); // All values are set to 0
      

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
      newChange.Rot = (IsKeyPressed(KeyCode.R)) ? new Vec3<double>(0, 0, 200.0) : (IsKeyPressed(KeyCode.E)) ? new Vec3<double>(0, 0, -200.0) : new Vec3<double>(0, 0, 0);


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


    /*!*********************************************************************
    \brief
      .Function to check if a key is triggered
    \params
      KEY_CODE key
      Enum of the key you want to check
    \return
      returns true is key is triggered
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyTriggered(KeyCode key);

    /*!*********************************************************************
    \brief
      .Function to check if a key is held
    \params
      KEY_CODE key
      Enum of the key you want to check
    \return
      returns true is key is held
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyHeld(KeyCode key);


    /*!*********************************************************************
    \brief
      .Function to check if a key is released
    \params
      KEY_CODE key
      Enum of the key you want to check
    \return
      returns true is key is relased in this frame
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyReleased(KeyCode key);

    /*!*********************************************************************
    \brief
      .Function to check if a key is Pressed
    \params
      KEY_CODE key
      Enum of the key you want to check
    \return
      returns true is key is Pressed
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static bool IsKeyPressed(KeyCode key);


    /*!*********************************************************************
		\brief
			function to Set the value of an entity's trnsform component. This function will be added as internal call
			to allow c# script to set entities' transform

		\params ID
			ID of the entity

		\params newValue
			values to be added to the entity's transform
		************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static void SetPosition(uint ID, Vec3<double> newPos);

    /*!*********************************************************************
		\brief
			function to Set the value of an entity's trnsform component. This function will be added as internal call
			to allow c# script to set entities' transform

		\params ID
			ID of the entity

		\params newValue
			values to be added to the entity's transform
		************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static void SetRotation(uint ID, Vec3<double> newRot);


    /*!*********************************************************************
		\brief
			function to Set the value of an entity's trnsform component. This function will be added as internal call
			to allow c# script to set entities' transform

		\params ID
			ID of the entity

		\params newValue
			values to be added to the entity's transform
		************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static void SetScale(uint ID, Vec3<double> newScale);



    /*!*********************************************************************
		\brief
			.Function to check the yoffset of the mouse scroll for the current frame
		\return
			yoffset of mouse scroll
		************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static double GetMouseScrollY();

    /*!*********************************************************************
    \brief
      .Function to check the xoffset of the mouse scroll for the current frame
    \return
      xoffset of  mouse scroll
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern static double GetMouseScrollX();

  }
}
