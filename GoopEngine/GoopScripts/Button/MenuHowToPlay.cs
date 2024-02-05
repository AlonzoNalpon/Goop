using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class MenuHowToPlay : IButtonClick, IButtonHoverEnter, IButtonHoverExit
  {
    public int PauseMenuID, DeeperPauseMenuID;

    public void OnClick(uint entity)
    {
      Utils.DeeperPauseMenu(PauseMenuID, DeeperPauseMenuID);
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
