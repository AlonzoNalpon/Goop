using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class TempEndTurn : Entity, IButtonClick
  {

    public TempEndTurn(uint entityID) : base(entityID)
    {

    }
    public void OnClick(uint entity)
    {
      Gameplay.GameManager gm = (Gameplay.GameManager) Utils.GetGameSysScript("GameSystem");
      gm.EndTurn();
    }
  }
}
