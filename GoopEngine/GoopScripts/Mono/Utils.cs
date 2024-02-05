/*!************************************************************************
\file Utils.cs
\author Han Qin Ding

\brief
C# script containing all the internal function calls
**************************************************************************/
using GoopScripts.Input;
using Microsoft.SqlServer.Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Xml.Linq;


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
      Function to get the value of an entity's trnsform component. This
      function will be added as internal call to allow c# script to set 
      entities' transform
    \params ID
      ID of the entity
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static Vec3<double> GetPosition(uint ID);

    /*!*********************************************************************
    \brief
      Function to get the value of an entity's transform component. This 
      function will be added as internal call to allow c# script to set 
      entities' transform
    \params ID
      ID of the entity
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static Vec3<double> GetWorldPosition(uint ID);

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
    extern public static void SetRotation(uint ID,  Vec3<double> newRot);

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

    /*!*********************************************************************
		\brief
		  Gets the object instance of a script given the entity and script name
		\param entityName
			The name of the entity
		\param scriptName
			The name of the script
		\return
		  The object instance
		************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static object GetScript(string entityName, string scriptName);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static object GetScriptFromID(uint entity, string scriptName);


    /*!*********************************************************************
		\brief
		  Gets the object instance of the game system script given the
			entity's name
		\param gameSysEntityName
			Name of the GameSystem entity
		\return
		  The object instance
		************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static object GetGameSysScript(string gameSysEntityName);



    /*!*********************************************************************
    \brief
      .Function to get the scriptInstance/MonoObject of an entity
    \params
      KEY_CODE key
      Enum of the key you want to check
    \return
      returns true is key is triggered
    ************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static object GetScriptInstance(uint entityID, string scriptName);

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


    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void StartAI(uint entityID);


    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void EndAI(uint entityID);

    //[MethodImplAttribute(MethodImplOptions.InternalCall)]
    //extern public static void SetCurrentRunningNode(uint currID);


    // /*!*********************************************************************
    //
    //    Functions For Game System
    //
    //************************************************************************/

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static double GetAnimationTime(string animName);


    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void PlayAnimation(string animName, uint entity);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void PlaySound(int soundIterator, uint entity);

    public enum ChannelType
    {
			BGM,
			SFX,
			VOICE,
			TOTAL_CHANNELS,
		}

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void PlaySoundF(string soundName, float volume, ChannelType channel, bool looped);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void StopSound(string soundName);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void StopChannel(ChannelType channel);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void GameSystemResolved();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SendString(string s);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetCardToQueuedState(uint entity, Vec3<double> target);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetCardToHandState(uint entity);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetBuffIconTextActive(uint entity, bool state);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void TransitionToScene(string scene);


    // /*!*********************************************************************
    // Pause Screens
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static bool GetPaused();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static bool GetDeeperPaused();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static bool SetPaused(bool active);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static bool SetDeeperPaused(bool active);


    // /*!*********************************************************************
    //
    //    Functions For UI (HEALTHBAR)
    //
    //************************************************************************/
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static bool GetLoseFocus();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetLoseFocus(bool active);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetIsActiveEntity(uint ID, bool active);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static bool GetIsActiveEntity(uint ID);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static uint SpawnPrefab(string key, Vec3<double> pos = new Vec3<double>());

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static uint GetObjectWidth(uint ID);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static uint GetObjectHeight(uint ID);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static uint CreateObject(string name,
      Vec3<double> pos,
      Vec3<double> scale,
      Vec3<double> rotation,
      uint ParentID = uint.MaxValue);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void UpdateSprite(uint ID, string name);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetTextComponent(int entity, string text, float alpha = 1.0f);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void CrossFadeAudio(string audio1, float startVol1, float endVol1, float normalizedFadeStart1, float normalizedFadeEnd1,
																						 string audio2, float startVol2, float endVol2, float normalizedFadeStart2, float normalizedFadeEnd2,
																						 float fadeDuration);
		public static void PauseMenu(int pauseMenu)
    {
      UI.PauseManager.SetPauseState(1);
      Utils.SetIsActiveEntity((uint)pauseMenu, true);
    }

    public static void UndeeperPause(int pauseMenu, int deeperPauseMenu)
    {
      UI.PauseManager.SetPauseState(1);
      Utils.SetIsActiveEntity((uint)deeperPauseMenu, false);
      Utils.SetIsActiveEntity((uint)pauseMenu, true);
    }

    public static void UnpauseMenu(int pauseMenu)
    {
      UI.PauseManager.SetPauseState(0);
      Utils.SetIsActiveEntity((uint)pauseMenu, false);
    }

    public static void DeeperPauseMenu(int pauseMenu, int deeperPauseMenu)
    {
      UI.PauseManager.SetPauseState(2);
      Utils.SetIsActiveEntity((uint)deeperPauseMenu, true);
      Utils.SetIsActiveEntity((uint)pauseMenu, false);
    }

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static void SetParent(uint parent, uint child);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    extern public static uint GetParentEntity(uint child);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static uint GetEntity(string entityName);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void DestroyEntity(uint entity);
	}
}