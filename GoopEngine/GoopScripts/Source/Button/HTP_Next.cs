using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace GoopScripts.Button
{
  internal class HTP_Next : IButtonClick
  {
    public HTP_Next() { }
    public void OnClick(uint entity)
    {
      if (HTP.m_page == 10)
      {
        return;
      }

      int m_prev = (HTP.m_page)++;

      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{m_prev}"), false);
      Utils.SetIsActiveEntity(Utils.GetEntity($"Page_{HTP.m_page}"), true);
    }
  }
}
