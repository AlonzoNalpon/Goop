/*!*********************************************************************
\file   RootNode.cs
\author han.q@digipen.edu
\date   15-March-2024
\brief  C# script attached to a Root node.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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

  internal class RootNode
  {
    List<uint> m_childID = new List<uint>();
    private uint m_nodeID = 0;
    //uint m_parentID;


    /*!*********************************************************************
  \brief
    Non default constructor of RootNode class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public RootNode(uint currID, uint _, uint[] temp, uint tempSize)
    {
      m_nodeID = currID;
      for (uint i = 0; i < tempSize; i++)
      {
        m_childID.Add(temp[i]);
      }
    }


    /*!*********************************************************************
    \brief
     Update function for the RootNode script. This function is called every frame
     if the script is attached to a root node
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      if (m_childID.Count > 0)
      {
        SetResult((int)NODE_STATES.STATE_RUNNING, m_nodeID);
        //Console.WriteLine("RUN NODE: " + m_childID[0]);
        RunChildNode(m_childID[0]);
        NODE_STATES childResult = (NODE_STATES)GetChildResult();

        if (childResult == NODE_STATES.STATE_SUCCEED)
        {
          ResetNode();
        }
        else if (childResult == NODE_STATES.STATE_FAILED)
        {
          ResetNode();
        }
        else
        {
          SetResult((int)NODE_STATES.STATE_WAITING, m_nodeID);
        }
      }

    }

    /*!*********************************************************************
    \brief
     ReturnFromChild function for the RootNode script. This function is called when the child node
    returns to this node.

    \param[entityID] uint
    ID of the entity
    ************************************************************************/
    public void ReturnFromChild(uint entityID)
    {
      OnExit();
    }

    /*!*********************************************************************
    \brief
    OnExit function for the RootNode script. This function is called when we finished running the whole treee
    and exit from the root node
    ************************************************************************/
    public void OnExit()
    {

      ResetNode();

    }




  }
}
