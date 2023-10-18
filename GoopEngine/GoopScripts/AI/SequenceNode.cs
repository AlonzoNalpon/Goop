using GoopScripts.Input;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.AI
{


  public class SequenceNode : MonoBehaviour
  {
    uint currentChild;
    uint currentID;

    /*!*********************************************************************
   \brief
     Non default constructor of Sequence Node cal

   \params enityID
    ID of the owner of this scipt
   ************************************************************************/
    public SequenceNode(uint nodeID = 0) : base(0)
    {
      currentID = nodeID;
      currentChild = 0;
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
    extern static void SetTransform(uint ID, Transform newValue);


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
