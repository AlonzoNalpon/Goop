using GoopScripts.Gameplay;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Cards
{
  internal class BlankCard : CardBase
  {
    public BlankCard(CardID cardID) : base(cardID)
    {
    }

    public override void Play(ref Stats source, ref Stats target)
    {
      // Does nothing
    }
  }
}
