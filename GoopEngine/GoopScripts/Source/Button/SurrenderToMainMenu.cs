using GoopScripts.Button;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.UI;

namespace GoopScripts.Button
{
	internal class SurrenderToMainMenu : TextButtonBase
	{
		/*!******************************************************************
		\brief
			Callback for when mouse release on the entity after click
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);
			Utils.PlayTransformAnimation(Utils.GetEntity("TransitionOut"), "MainMenu");
		}
	}
}
