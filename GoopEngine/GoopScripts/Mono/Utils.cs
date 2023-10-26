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
    //   /*!*********************************************************************
    //\brief
    //  .Function to check if a key is triggered
    //\params
    //  KEY_CODE key
    //  Enum of the key you want to check
    //\return
    //  returns true is key is triggered
    //************************************************************************/
    //   [MethodImplAttribute(MethodImplOptions.InternalCall)]
    //   extern static bool IsKeyTriggered(KeyCode key);

    //   /*!*********************************************************************
    //   \brief
    //     .Function to check if a key is held
    //   \params
    //     KEY_CODE key
    //     Enum of the key you want to check
    //   \return
    //     returns true is key is held
    //   ************************************************************************/
    //   [MethodImplAttribute(MethodImplOptions.InternalCall)]
    //   extern static bool IsKeyHeld(KeyCode key);


    //   /*!*********************************************************************
    //   \brief
    //     .Function to check if a key is released
    //   \params
    //     KEY_CODE key
    //     Enum of the key you want to check
    //   \return
    //     returns true is key is relased in this frame
    //   ************************************************************************/
    //   [MethodImplAttribute(MethodImplOptions.InternalCall)]
    //   extern static bool IsKeyReleased(KeyCode key);

    //   /*!*********************************************************************
    //   \brief
    //     .Function to check if a key is Pressed
    //   \params
    //     KEY_CODE key
    //     Enum of the key you want to check
    //   \return
    //     returns true is key is Pressed
    //   ************************************************************************/
    //   [MethodImplAttribute(MethodImplOptions.InternalCall)]
    //   extern static bool IsKeyPressed(KeyCode key);


    //   /*!*********************************************************************
    //	\brief
    //		.Function to check the yoffset of the mouse scroll for the current frame
    //	\return
    //		yoffset of mouse scroll
    //	************************************************************************/
    //   [MethodImplAttribute(MethodImplOptions.InternalCall)]
    //   extern static double GetMouseScrollY();

    //   /*!*********************************************************************
    //   \brief
    //     .Function to check the xoffset of the mouse scroll for the current frame
    //   \return
    //     xoffset of  mouse scroll
    //   ************************************************************************/
    //   [MethodImplAttribute(MethodImplOptions.InternalCall)]
    //   extern static double GetMouseScrollX();





    // /*!*********************************************************************
    //
    //    Functions For Behaviour Tree
    //
    //************************************************************************/

      public enum NODE_STATES{
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
      extern public static void SetResult(int result,uint currID);

      [MethodImplAttribute(MethodImplOptions.InternalCall)]
      extern public static void SetNewChildIndex(int index);

      [MethodImplAttribute(MethodImplOptions.InternalCall)]
      extern public static void RunChildNode(uint childNodeID);

      [MethodImplAttribute(MethodImplOptions.InternalCall)]
      extern public static void JumpToParent();

      [MethodImplAttribute(MethodImplOptions.InternalCall)]
      extern public static void ResetNode();

  }

}