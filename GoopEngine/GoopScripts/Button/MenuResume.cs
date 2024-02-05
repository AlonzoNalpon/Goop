using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class MenuResume : IButtonClick
  {
    public int PauseMenuID;

    public void OnClick(uint entity)
    {
      Utils.UnpauseMenu(PauseMenuID);
    }
  }
}
