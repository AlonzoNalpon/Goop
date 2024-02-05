using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  internal class ReturnFromQueue : IButtonClick
  {
    public ReturnFromQueue() { }

    public void OnClick(uint entity)
    {
      Stats player = (Stats)Utils.GetScript("Player", "Stats");
      uint cardId = Utils.GetParentEntity(entity);
      if (!player.m_deckMngr.IsEntityInQueue(cardId))
      {
        //Console.WriteLine("Card Not In Queue!");
        return;
      }

      Utils.PlaySoundF("SFX_CardPlay5", 1.0f, Utils.ChannelType.SFX, false);
      player.UnqueueCardByID(cardId);
    }
  }
}
