using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class BuffIconInteraction : IButtonHoverEnter, IButtonHoverExit
  {
    BuffIconInteraction() { }
    public void OnHoverEnter(uint entity)
    {
      Console.WriteLine("Hover");
      Utils.SetBuffIconTextActive(entity, true);
    }

    public void OnHoverExit(uint entity)
    {
      Utils.SetBuffIconTextActive(entity, false);
    }
  }
}
