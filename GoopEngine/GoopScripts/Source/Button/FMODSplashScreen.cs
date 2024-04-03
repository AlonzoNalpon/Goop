using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Button
{
  internal class FMODSplashScreen : IButtonClick, IAnimationEvent
  {
    /*!******************************************************************
		\brief
			Callback for when mouse click on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
    public void OnClick(uint entity)
    {
      Utils.TransitionToScene("LogoSplashScreen");
    }

    /*!******************************************************************
		\brief
			Callback for when an animation
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
    public void PlayEvent(uint entity)
    {
      OnClick(entity);
    }
  }
}
