/*!************************************************************************
\file IsWithinPlayerRange.cs
\author Han Qin Ding

\brief
C# script attached to a leaf node.
Used to check if the enemy is inside of the player's attack range.
If the penemy is inside of the player's attack range, it returns fail, else return success

**************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.AI.Enemy
{

  internal class IsWithinPlayerRange : MonoBehaviour
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of IsWithinPlayerRange class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public IsWithinPlayerRange(uint currID, uint parentID, uint[] temp, uint size) : base()
    {
      m_parentID = parentID;
      m_nodeID = currID;
      // Console.WriteLine("Is Within rangeID:" + m_nodeID);
    }


    /*!*********************************************************************
   \brief
     Awake function for the IsWithinPlayerRange script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the IsWithinPlayerRange script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     onUpdate function for the IsWithinPlayerRange script. This function is called every frame
     if the script is attached to a node
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      if (PlayerExist())
      {
        uint playerID = GetPlayerID();
        Vec3<double> playerPos = GetPosition(playerID);
        Vec3<double> currPos = GetPosition(entityID);
        double deltaX = currPos.X - playerPos.X;
        double deltaY = currPos.Y - playerPos.Y;
        double distance = Math.Sqrt(deltaX * deltaX + deltaY * deltaY);

        if (distance > 170.0)
        {
          OnFail();
        }
        else
        {
          OnSuccess();
        }
      }
      else
      {
        OnFail();
      }


    }

    /*!*********************************************************************
    \brief
     onFail function for the IsWithinPlayerRange script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the IsWithinPlayerRange script. This function is called when the script succeed.
    it informs the tree that this script succeed and jump back to the parent node
    ************************************************************************/
    public void OnSuccess()
    {
      SetResult((int)NODE_STATES.STATE_SUCCEED, m_nodeID);
      JumpToParent();
    }

    public void ReturnFromChild(uint entityID)
    {

    }

    public void OnExit()
    {

    }


    /*!*********************************************************************
    \brief
     Update function for the IsWithinPlayerRange script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void Update()
    {

    }

    /*!*********************************************************************
    \brief
     late Update function for the IsWithinPlayerRange script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }



  }
}
