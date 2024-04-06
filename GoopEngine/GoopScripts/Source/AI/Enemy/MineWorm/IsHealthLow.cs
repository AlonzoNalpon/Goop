/*!*********************************************************************
\file   IsHealthLow.cs
\author han.q@digipen.edu
\date   15-March-2024
\brief  C# script for enemyAi Tree
        Checks if enemy's health is low

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Mono;
using GoopScripts.Gameplay;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.AI.Enemy.MineWorm
{

  internal class IsHealthLow
  {
    private uint m_parentID = 0;
    private uint m_nodeID = 0;



    /*!*********************************************************************
  \brief
    Non default constructor of IsHealthLow class

  \params enityID
   ID of the owner of this scipt
  ************************************************************************/
    public IsHealthLow(uint currID, uint parentID, uint[] temp, uint size)
    {
      m_parentID = parentID;
      m_nodeID = currID;
    }

    /*!*********************************************************************
    \brief
     Update function for the IsHealthLow script. This function is called every frame
     if the script is attached to a leaf node
    
    \param[entityID] uint
    ID of the entity

    \param[dt] dobule
    delta time
    ************************************************************************/
    public void OnUpdate(uint entityID, double dt)
    {
      const int minHealth = 5;
      Stats EnemyStats = (Stats)GetScriptFromID(entityID, "Stats");
      if (EnemyStats.m_healthBar.m_health < minHealth)
      {
        OnSuccess();
      }
      else
      {
        OnFail();
      }


    }

    /*!*********************************************************************
    \brief
     onFail function for the IsHealthLow script. This function is called when the script fails.
    it informs the tree that this script failed and jump back to the parent node
    ************************************************************************/
    public void OnFail()
    {
      SetResult((int)NODE_STATES.STATE_FAILED, m_nodeID);
      JumpToParent();
    }

    /*!*********************************************************************
    \brief
     onSuccess function for the IsHealthLow script. This function is called when the script succeed.
    it informs the tree that this script succeed and jump back to the parent node
    ************************************************************************/
    public void OnSuccess()
    {
      SetResult((int)NODE_STATES.STATE_SUCCEED, m_nodeID);
      JumpToParent();
    }

  }
}
