using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.Gameplay
{
  public class GameManager
  {
    Random m_rng;
    CardManager m_cardManager;
    double m_animTime = 1.0; // hard coded for now
    double m_currTime;
    int m_numResolves;
    int m_currResolves = 0;
    bool newTurn = true;

    GameManager()
    {
      m_rng = new Random();
      m_cardManager = new CardManager();
    }

    // function to allow c++ to edit the list of cards in cardmanager
    // this should use cardmanager's c++ interface function

    public void OnUpdate(double dt, Stats player, uint playerEntity, Stats enemy, uint enemyEntity, uint playerHand, uint playerQueue, uint enemyQueue)
		{

      if (newTurn)
			{
				m_numResolves = player.m_cardQueue.Length >= enemy.m_cardQueue.Length ? player.m_cardQueue.Length : enemy.m_cardQueue.Length;
        newTurn = false;

				// Do 1 turn of stuff
				m_cardManager.Cards[(int)player.m_cardQueue[m_currResolves]].Play(ref player, ref enemy);
        m_cardManager.Cards[(int)enemy.m_cardQueue[m_currResolves]].Play(ref enemy, ref player);

        Utils.PlaySound(m_rng.Next(1, 1), playerEntity);
        Utils.SetQueueCardID(playerQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
        Utils.PlaySound(m_rng.Next(0, 1), enemyEntity);
        Utils.SetQueueCardID(enemyQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
        //play anim?
        Utils.SendString("hi friend");
        Utils.PlayAnimation("SS_LeahShoot", playerEntity);

        // remove card
        player.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
        enemy.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
        ++m_currResolves;
			}

			bool shouldEnd = false;
      if (m_currTime > m_animTime)
      {
        // Overflow the time
        m_currTime = m_animTime - m_currTime;

				// Do 1 turn of stuff
				m_cardManager.Cards[(int)player.m_cardQueue[m_currResolves]].Play(ref player, ref enemy);
				m_cardManager.Cards[(int)enemy.m_cardQueue[m_currResolves]].Play(ref enemy, ref player);

        //play anim? play sound?
        Utils.PlaySound(m_rng.Next(0, 1), playerEntity);
        Utils.SetQueueCardID(playerQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);
        Utils.PlaySound(m_rng.Next(0, 1), enemyEntity);
        Utils.SetQueueCardID(enemyQueue, m_currResolves, (int)CardBase.CardID.NO_CARD);

        // remove card
        player.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;
        enemy.m_cardQueue[m_currResolves] = CardBase.CardID.NO_CARD;

        ++m_currResolves;

				if (m_currResolves >= m_numResolves)
				{
					shouldEnd = true;
				}
      }
      else
      {
				m_currTime += dt;
			}

      if (shouldEnd)
			{
        // Draw cards here.        
				Utils.GameSystemResolved();
        int i = 0;
        for (; i < player.m_hand.Length; ++i)
        {
          if (player.m_hand[i] == CardBase.CardID.NO_CARD)
          {
            break;
          }
        }
        // Draw card if there is space, if no space burn card
        if (i < player.m_hand.Length && player.m_deck.Length > 0)
        {
          int drawnCard = m_rng.Next(0, player.m_deck.Length - 1);

          List<CardBase.CardID> temp = player.m_deck.ToList();
          temp.Remove((CardBase.CardID)drawnCard);
          player.m_deck = temp.ToArray();

          Utils.SetHandCardID(playerHand, i, drawnCard);
        }
        m_currResolves = 0;
        m_currTime = 0;
        newTurn = true;
			}
    }
  }
}
