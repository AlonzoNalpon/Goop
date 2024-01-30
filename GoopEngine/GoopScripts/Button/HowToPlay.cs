using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class HowToPlay : IButtonClick, IButtonHoverEnter, IButtonHoverExit
  {
    public int PauseMenuID;
    public int DeeperPauseMenuID;

    public HowToPlay()
    {

    }

    public void OnClick(uint entity)
    {
      Console.WriteLine("test");
      //Utils.SetIsActiveEntity((uint)DeeperPauseMenuID, true);
      //Utils.SetIsActiveEntity((uint)PauseMenuID, false);
      //UI.PauseManager.SetDeeperPause(true);
    }

    public void OnHoverEnter(uint entity)
    {
      Console.WriteLine(entity.ToString() + "Enter");
    }

    public void OnHoverExit(uint entity)
    {
      Console.WriteLine(entity.ToString() + "Exit");
    }
  }
}
