using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class EndTurn : IButtonClick
  {
    public int m_player;
    public void OnClick(uint entity)
    {
      Console.WriteLine("test" + m_player);
    }
  }
}
