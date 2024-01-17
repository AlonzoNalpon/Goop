using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Permissions;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.UI
{
  public class HealthBar
  {
    // static private readonly int PADDING_SIZE = 5;
    private int health = 10;
    // private int maxHealth = 10;
    public int ID;
    public int m_width = 300;
    
    public HealthBar() 
    {
      // Utils.SpawnPrefab("TestButton");
      // calculate how many bar to spawn
      // generate bar in the bar
    }

    public void DecreaseHealth(int amount)
    {
      health -= amount;
    }
  }
}
