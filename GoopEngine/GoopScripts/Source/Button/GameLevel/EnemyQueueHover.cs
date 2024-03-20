/*!*********************************************************************
\file   EnemyQueueHover.cs
\author chengen.lau\@digipen.edu
\date   15-March-2024
\brief  This script is attached to enemy card icons in queue to
        facilitate the displaying of the card prefab upon hovering over
        icons in the queue.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  internal class EnemyQueueHover : IButtonHoverEnter, IButtonHoverExit
  {
    /*!*********************************************************************
    \brief
      Calls upon QueueCardDisplay class to display the card prefab
    \param entity
      The entity id of the icon in queue
    ************************************************************************/
    public void OnHoverEnter(uint entity)
    {
      QueueCardDisplay.ShowEnemyCard(entity);
    }

    /*!*********************************************************************
    \brief
      Calls upon QueueCardDisplay class to destroy the card prefab
    \param entity
      The entity id of the icon in queue
    ************************************************************************/
    public void OnHoverExit(uint entity)
    {
      QueueCardDisplay.DestroyCard();
    }
  }
}
