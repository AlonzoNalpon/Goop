using GoopScripts.Gameplay;
using GoopScripts.Mono;
using GoopScripts.Source.Button.Tutorial;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  internal class HTP_Back : ArrowButton
  {
    public override void OnRelease(uint entity)
		{
			base.OnRelease(entity);
			if (HTP.m_page == 1)
      {
        return;
      }

      int m_prev = (HTP.m_page)--;

      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{m_prev}"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP.m_page}"), true);
    }
  }
}
