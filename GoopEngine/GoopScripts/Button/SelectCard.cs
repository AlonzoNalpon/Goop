using GoopScripts.Gameplay;
using GoopScripts.Mono;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
  public class SelectCard : IButtonClick
  {
    public SelectCard() { }

    public void OnClick(uint entity)
    {
      Stats player = (Stats)Utils.GetScript("Player", "Stats");

      if (!player.m_deckMngr.IsEntityInHand(entity))
      {
        //Console.WriteLine("Card Not In Hand!");
        return;
      }
      Utils.PlaySoundF("SFX_CardPlay5", 1.0f, Utils.ChannelType.SFX, false);
      player.QueueCardByID(entity);
    }
  }
}
