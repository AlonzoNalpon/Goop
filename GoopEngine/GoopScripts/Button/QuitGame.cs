using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class QuitGame : TextButtonBase
  {
    public override void OnRelease(uint entity)
		{
      base.OnRelease(entity);
      Utils.DispatchQuitEvent();
    }
  }
}
