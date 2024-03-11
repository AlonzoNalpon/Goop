﻿/*!************************************************************************
\file LeafNode.cs
\author Han Qin Ding

\brief
C# script attached to a leaf node.
Used by leaf node that always returns true (for testing)

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

  internal class LeafNode
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of LeafNode class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public LeafNode(uint currID, uint parentID, uint[] temp, uint size)
    {
      m_parentID = parentID;
      m_nodeID = currID;

    }


    /*!*********************************************************************
   \brief
     Awake function for the LeafNode script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the LeafNode script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the LeafNode script. This function is called every frame
     if the script is attached to an entity
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      //Console.WriteLine("RUN LEAFNODE");
      OnSuccess(entityID);

    }

    /*!*********************************************************************
    \brief
     onFail function for the LeafNode script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail(uint entityID)
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
     \brief
      onSuccess function for the LeafNode script. This function is called when the script succeed.
     it informs the tree that this script succeed and jump back to the parent node
     ************************************************************************/
    public void OnSuccess(uint entityID)
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
     Update function for the LeafNode script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void Update()
    {

    }

    /*!*********************************************************************
    \brief
     late Update function for the LeafNode script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }



  }
}