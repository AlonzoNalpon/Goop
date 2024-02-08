using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class DisableEndTurn : Entity
  {
    DisableEndTurn(uint entity) : base(entity) { }

    public void OnUpdate(double deltaTime)
    {
      if (!GameManager.IsResolutionPhase())
      {
        Utils.SetIsActiveEntity(entityID, false);
      }
    }
  }
}
