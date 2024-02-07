using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class MenuMainMenu : IButtonClick
  {
    public void OnClick(uint entity)
    {
      Utils.ToMainMenu();
    }
  }
}
