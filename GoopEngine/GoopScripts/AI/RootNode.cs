using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.AI
{

  internal class RootNode : MonoBehaviour
  {
    List<uint> m_childID = new List<uint>();
    private uint m_nodeID = 0;
    //uint m_parentID;


    /*!*********************************************************************
  \brief
    Non default constructor of Player class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public RootNode(uint currID, uint _, uint[] temp, uint tempSize) : base()
    {
      m_nodeID = currID;
      for (uint i = 0; i < tempSize; i++)
      {
        m_childID.Add(temp[i]);
      }
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
      //Console.WriteLine("Run RootNode\n");
      //Run the child node 
      //Console.WriteLine("START TREE\n");
      if (m_childID.Count > 0)
      {
        SetResult((int)NODE_STATES.STATE_RUNNING, m_nodeID);
        RunChildNode(m_childID[0]);
        NODE_STATES childResult = (NODE_STATES)GetChildResult();

        if (childResult == NODE_STATES.STATE_SUCCEED)
        {
          // Console.WriteLine("CLEARED THE TREE!\n");
          //Console.WriteLine("CHILD SUCCEED!\n");
          ResetNode();
        }
        else if (childResult == NODE_STATES.STATE_FAILED)
        {
          //Console.WriteLine("CLEARED THE TREE!\n");
          //Console.WriteLine("CHILD FAILED!\n");
          ResetNode();
        }
        else
        {
          //Console.WriteLine("RootNode Waiting\n");
          SetResult((int)NODE_STATES.STATE_WAITING, m_nodeID);
        }
      }

    }
    public void OnFail(uint entityID)
    {
      //SetTheNextNode and the result

      //SetNewCurrentNodeID(m_parentID);
      //SetResult(false);


    }

    public void OnSuccess(uint entityID)
    {
      //SetTheNextNode and the result

      //SetNewCurrentNodeID(m_parentID);
      //SetResult(true);
    }

    public void ReturnFromChild(uint entityID)
    {
      // Console.WriteLine("CLEARED THE TREE!\n");
      OnExit();
    }

    public void OnExit()
    {

      ResetNode();

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
