using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class MenuQuitGame : IButtonClick
  {
    public int Command;
    public int PauseMenuID, DeeperPauseMenuID;
    public void OnClick(uint entity)
    {
      if (Command == -1)
      {
        // Return to main menu
        return;
      }
      Utils.DeeperPauseMenu(PauseMenuID,DeeperPauseMenuID);
    }
  }
}
