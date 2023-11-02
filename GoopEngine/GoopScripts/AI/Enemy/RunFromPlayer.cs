/*!************************************************************************
\file RunFromPlayer.cs
\author Han Qin Ding

\brief
C# script attached to a leaf node.
Used to move the enemy away from the player

**************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.AI
{

  internal class RunFromPlayer : MonoBehaviour
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of RunFromPlayer class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public RunFromPlayer(uint currID, uint parentID, uint[] temp, uint size) : base()
    {
      m_parentID = parentID;
      m_nodeID = currID;
      //Console.WriteLine("RUN fromplayerID:" + m_nodeID);

    }


    /*!*********************************************************************
   \brief
     Awake function for the RunFromPlayer script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the RunFromPlayer script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the RunFromPlayer script. This function is called every frame
     if the script is attached to a leaf node
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      if (PlayerExist())
      {
        //Console.WriteLine("Run Move awway\n");
        uint playerID = GetPlayerID();
        Vec3<double> playerPos = GetPosition(playerID);
        Vec3<double> currPos = GetPosition(entityID);
        double deltaX = currPos.X - playerPos.X;
        double deltaY = currPos.Y - playerPos.Y;
        double distance = Math.Sqrt(deltaX * deltaX + deltaY * deltaY);


        SetPosition(entityID, new Vec3<double>((deltaX / distance) * 300, (deltaY / distance) * 300, 0.0));

        OnSuccess();
      }
      else
      {
        OnFail();
      }

    }
    /*!*********************************************************************
    \brief
     onFail function for the RunFromPlayer script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the RunFromPlayer script. This function is called when the script succeed.
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



  }
}
