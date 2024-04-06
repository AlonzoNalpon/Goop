/*!*********************************************************************
\file   EnemyAIStruct.cs
\author han.q@digipen.edu
\date   15-March-2024
\brief  C# script that contains the struct for making the MinMax AI
        algorithm the struct will contain all the data required for 1
        permutation.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Cards;
using GoopScripts.Gameplay;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Source.Mono
{
  public struct Permutation
  {
    public int m_turnCount { get; set; }

    //health
    public int m_playerHealth { get; set; }
    public int m_enemyHealth { get; set; }

    //Cards played 
    public List<CardBase.CardID> m_cardPlayed { get; set; }
    public List<CardBase.CardID> m_cardstoPlay { get; set; }

    //Deck
    public  Deck m_playerDeck;
    public  Deck m_enemyDeck;

    //Hand
    public List<ValueTuple<CardBase.CardID, uint>> m_playerHand { get; set; }
    public List<ValueTuple<CardBase.CardID, uint>> m_enemyHand { get; set; }

    //Discard
    public List<CardBase.CardID> m_playerDiscard { get; set; }
    public List<CardBase.CardID> m_enemyDiscard { get; set; }

    //Buff
    public List<Buff> m_playerBuff { get; set; }
    public List<Buff> m_enemyBuff { get; set; }


    public Permutation(Permutation p)
    {
      m_turnCount = p.m_turnCount;
      m_playerHealth = p.m_playerHealth;
      m_enemyHealth = p.m_enemyHealth;

      //Cards played 
      m_cardPlayed  = new List<CardBase.CardID>(p.m_cardPlayed);
      m_cardstoPlay = new List<CardBase.CardID>(p.m_cardstoPlay);

      //Deck
      m_playerDeck = new Deck(p.m_playerDeck);
      m_enemyDeck = new Deck(p.m_enemyDeck);

      //Hand
      m_playerHand = new List<ValueTuple<CardBase.CardID, uint>>(p.m_playerHand);
      m_enemyHand = new List<ValueTuple<CardBase.CardID, uint>>(p.m_enemyHand);

      //Discard
      m_playerDiscard = new List<CardBase.CardID>(p.m_playerDiscard);
      m_enemyDiscard = new List<CardBase.CardID>(p.m_enemyDiscard);

      //Buff
      m_playerBuff = new List<Buff>(p.m_playerBuff);
      m_enemyBuff = new List<Buff>(p.m_enemyBuff);
    }
  }

 }
