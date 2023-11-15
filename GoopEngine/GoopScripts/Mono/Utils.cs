/*!************************************************************************
\file Utils.cs
\author Han Qin Ding

\brief
C# script containing all the internal function calls
**************************************************************************/
using GoopScripts.Input;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;


namespace GoopScripts.Mono
{
  public static class Utils
  {

    // /*!*********************************************************************
    //
    //    Helper Functions
    //
    //************************************************************************/


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
    extern public static bool IsKeyTriggered(KeyCode key);

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
    extern public static bool IsKeyHeld(KeyCode key);


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
    extern public static bool IsKeyReleased(KeyCode key);

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
    extern public static bool IsKeyPressed(KeyCode key);

    /*!*********************************************************************
    \brief
      .Function to check the yoffset of the mouse scroll for the current frame
    \return
      yoffset of mouse scroll
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static double GetMouseScrollY();

    /*!*********************************************************************
    \brief
      .Function to check the xoffset of the mouse scroll for the current frame
    \return
      xoffset of  mouse scroll
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static double GetMouseScrollX();

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
    extern public static Vec3<double> GetPosition(uint ID);

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
    extern public static Vec3<double> GetRotation(uint ID);


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
    extern public static Vec3<double> GetScale(uint ID);


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
    extern public static void SetPosition(uint ID, Vec3<double> newPos);

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
    extern public static void SetRotation(uint ID, Vec3<double> newRot);


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
    extern public static void SetScale(uint ID, Vec3<double> newScale);

    // /*!*********************************************************************
    //
    //    Functions For Behaviour Tree
    //
    //************************************************************************/

    public enum NODE_STATES
    {
      STATE_NEW,
      STATE_RUNNING,
      STATE_WAITING,
      STATE_SUCCEED,
      STATE_FAILED,
      STATE_COUNT
    }

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static int GetChildResult();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static int GetCurrentChildIndex();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetResult(int result, uint currID);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetNewChildIndex(int index);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void RunChildNode(uint childNodeID);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void JumpToParent();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void ResetNode();





    // /*!*********************************************************************
    //
    //    Functions For Enemy
    //
    //************************************************************************/


    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static uint GetPlayerID();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static bool PlayerExist();

		//[MethodImplAttribute(MethodImplOptions.InternalCall)]
		//extern public static void SetCurrentRunningNode(uint currID);


		// /*!*********************************************************************
		//
		//    Functions For Cards
		//
		//************************************************************************/
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static uint GetHealth(uint entity);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void SetHealth(uint entity, uint health);
	}

}