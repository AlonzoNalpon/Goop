/*!************************************************************************
\file SequenceNode.cs
\author Han Qin Ding

\brief
C# script attached to a composite node.
Will each child node every frame.
if any of the child node fails, it will not run the rest of the child nodes
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

  internal class SequenceNode
  {
    private uint m_parentID = 0;
    List<uint> m_childID = new List<uint>();
    private int m_currentIndex = 0;
    private uint m_nodeID = 0;


    /*!*********************************************************************
  \brief
    Non default constructor of SequenceNode class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public SequenceNode(uint currID, uint parentID, uint[] temp, uint tempSize)
    {
      m_nodeID = currID;
      m_parentID = parentID;
      for (uint i = 0; i < tempSize; i++)
      {
        m_childID.Add(temp[i]);
      }
    }



    /*!*********************************************************************
   \brief
     Awake function for the SequenceNode script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the SequenceNode script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the SequenceNode script. This function is called every frame
     if the script is attached to a cmposite node
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      SetResult((int)NODE_STATES.STATE_RUNNING, m_nodeID);
      //Get current position Index
      m_currentIndex = GetCurrentChildIndex();

      //Run the child node 
      if (m_childID.Count > m_currentIndex && m_childID.Count > 0)
      {
        //Run the child node 
        RunChildNode(m_childID[m_currentIndex]);

        NODE_STATES childResult = (NODE_STATES)GetChildResult();


        if (childResult == NODE_STATES.STATE_SUCCEED)
        {
          m_currentIndex = GetCurrentChildIndex() + 1;
          SetNewChildIndex(m_currentIndex);
          if (m_currentIndex >= m_childID.Count)
          {
            OnSuccess(entityID);
          }
          else
          {
            SetResult((int)NODE_STATES.STATE_WAITING, m_nodeID);
          }
        }
        else if (childResult == NODE_STATES.STATE_FAILED)
        {
          OnFail(entityID);
        }
        else
        {

          SetResult((int)NODE_STATES.STATE_WAITING, m_nodeID);
        }
      }
      else
      {
        OnFail(entityID);
      }




    }

    /*!*********************************************************************
    \brief
     onFail function for the SequenceNode script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail(uint entityID)
    {
      //SetTheNextNode and the result

      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      m_currentIndex = 0;
      JumpToParent();


    }
    /*!*********************************************************************
     \brief
      onSuccess function for the SequenceNode script. This function is called when the script succeed.
     it informs the tree that this script succeed and jump back to the parent node
     ************************************************************************/

    public void OnSuccess(uint entityID)
    {
      //SetTheNextNode and the result
      SetResult((int)NODE_STATES.STATE_SUCCEED, m_nodeID);
      m_currentIndex = 0;
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     ReturnFromChild function for the SequenceNode script. This function is called when the child node
    returns to this node.

    \param[entityID] uint
    ID of the entity
    ************************************************************************/
    public void ReturnFromChild(uint entityID)
    {
      //It common for the tree to only travel back to the composite node in a later frame
      //Hence this function is needed to maintain the flow of the tree
      NODE_STATES childResult = (NODE_STATES)GetChildResult();
      if (childResult == NODE_STATES.STATE_SUCCEED)
      {
        m_currentIndex = GetCurrentChildIndex() + 1;
        SetNewChildIndex(m_currentIndex);
        if (m_currentIndex >= m_childID.Count)
        {
          OnSuccess(entityID);
        }
        SetResult((int)NODE_STATES.STATE_WAITING, m_nodeID);

      }
      else if (childResult == NODE_STATES.STATE_FAILED)
      {
        OnFail(entityID);
      }
      else
      {
        SetResult((int)NODE_STATES.STATE_WAITING, m_nodeID);
      }

    }

    public void OnExit()
    {

    }


    /*!*********************************************************************
    \brief
     Update function for the SequenceNode script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void Update()
    {

    }

    /*!*********************************************************************
    \brief
     late Update function for the SequenceNode script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }



  }
}
