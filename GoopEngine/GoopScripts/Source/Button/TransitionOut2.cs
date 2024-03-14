using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Button
{
  public class TransitionOut2 : TextButtonBase // in the event there is 2 outgoing buttons
  {
    /*!******************************************************************
		\brief
			Callback for when mouse is released after clicking the entity
		
		\param entity
			Entity ID of the entity calling the event
		********************************************************************/
    public override void OnRelease(uint entity)
    {
      base.OnRelease(entity);
      Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "TransitionOut2");
    }
  }
}
