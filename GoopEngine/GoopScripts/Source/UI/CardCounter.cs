using GoopScripts.Cards;
using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.UI
{
  public class CardCounter : Entity
  {
    public int m_cardID;
    public CardCounter(uint entity) : base(entity)
    {
    }

    public void OnCreate()
    {
      Console.WriteLine(m_cardID + ": " + HomeBase.GetCount((CardBase.CardID)m_cardID));
      Console.WriteLine((int)Utils.GetChildEntity(entityID, "Text"));
      Console.WriteLine("x" + HomeBase.GetCount((CardBase.CardID)m_cardID));

      Utils.SetTextComponent((int)Utils.GetChildEntity(entityID, "Text"), "x" + HomeBase.GetCount((CardBase.CardID)m_cardID));
      
    }

  }
}
