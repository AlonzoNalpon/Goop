using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.AI
{

  internal class MoveToPlayer : MonoBehaviour
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of Player class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public MoveToPlayer(uint currID, uint parentID, uint[] temp, uint size) : base()
    {
      m_parentID = parentID;
      m_nodeID = currID;
      //Console.WriteLine("Move to playerID:" + m_nodeID);

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
    public void OnUpdate(uint entityID, double dt)
    {
      Console.WriteLine("Run Move to\n");

      if (PlayerExist())
      {
        uint playerID = GetPlayerID();
        Vec3<double> playerPos = GetPosition(playerID);
        Vec3<double> currPos = GetPosition(entityID);
        double deltaX = playerPos.X - currPos.X;
        double deltaY = playerPos.Y - currPos.Y;
        double distance = Math.Sqrt(deltaX * deltaX + deltaY * deltaY);


        SetPosition(entityID, new Vec3<double>((deltaX / distance) * 300, (deltaY / distance) * 300, 0.0));
        OnSuccess();
      }
      else
      {
        OnFail();
      }

    }
      public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

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
