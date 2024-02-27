using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  internal class EnemyQueueHover : IButtonHoverEnter, IButtonHoverExit
  {
    public void OnHoverEnter(uint entity)
    {
      QueueCardDisplay.ShowEnemyCard(entity);
    }

    public void OnHoverExit(uint entity)
    {
      QueueCardDisplay.DestroyCard();
    }
  }
}
