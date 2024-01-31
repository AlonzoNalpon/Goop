using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class MenuClose : IButtonClick, IButtonHoverEnter, IButtonHoverExit
  {
    public int PauseMenuID;
    public int DeeperPauseMenuID;
    public MenuClose() { }

    public void OnClick(uint entity)
    {

      switch (UI.PauseManager.GetPauseState())
      {
        case 0:
          Utils.PauseMenu((uint)PauseMenuID);
          break;
        case 1:
          Utils.UnpauseMenu((uint)PauseMenuID);
          break;
        case 2:
          Utils.UndeeperPause((uint)PauseMenuID, (uint)DeeperPauseMenuID);
          break;
        default:
          break;
      }
    }
    public void OnHoverEnter(uint entity)
    {
      // Console.WriteLine(entity.ToString() + "Enter");
    }

    public void OnHoverExit(uint entity)
    {
      // Console.WriteLine(entity.ToString() + "Exit");
    }

  }
}
