﻿/*!*********************************************************************
\file   ComboManager.cs
\author chengen.lau\@digipen.edu
\co-author c.phua\@digipen.edu
\co-author Han Qin Ding
\date   10-January-2024
\brief  
Resolves card combos and applied the effect to the respective characters.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;

namespace GoopScripts.Gameplay
{
  // a pair or tuple where both elements are of the same type
  // and ordering doesn't matter when compared for equality
  using ComboPair = Mono.UnorderedPair<CardBase.CardType>;

  public static class ComboManager
  {
    enum ComboType
    {
      ATK_UP,
      BLINDED,
      BLOCK_UP,
      TURN_SKIPPED
    }

    static Dictionary<ComboPair, Combo> m_comboList = new Dictionary<ComboPair, Combo>();

    // To add an entry into the list of combos:
    //  1. Define a ComboPair as ComboPair(TYPE_1, TYPE_2)
    //  2. Define a Buff to add (List of buffs i.e. List<Buff> can also be passed in)
    //  3. Create the Combo Object (currently exists BuffCombo, DebuffCombo or DrawCombo)
    //  4. Do m_combolist.Add(ComboPair, Combo or ComboList)
    static ComboManager()
    {
      m_comboList.Add(new ComboPair(CardBase.CardType.ATTACK, CardBase.CardType.ATTACK),
        new BuffCombo("Atk Up", new Buff(Buff.BuffType.FLAT_ATK_UP, 1.0f, 1)));

      m_comboList.Add(new ComboPair(CardBase.CardType.ATTACK, CardBase.CardType.BLOCK),
        new DrawCombo("Draw +1"));

      m_comboList.Add(new ComboPair(CardBase.CardType.BLOCK, CardBase.CardType.BLOCK),
        new BuffCombo("Block Up", new Buff(Buff.BuffType.INCREASE_BLOCK, 1.0f, 1)));

      m_comboList.Add(new ComboPair(CardBase.CardType.BLOCK, CardBase.CardType.SPECIAL),
        new BuffCombo("Chance to Skip Enemy Turn", new Buff(Buff.BuffType.SKIP_TURN, 0.0f, 1), 50));


      Buff blind = new Buff(Buff.BuffType.BLIND, 1.0f, 1);
      Buff atkUp = new Buff(Buff.BuffType.FLAT_ATK_UP, 1.0f, 1);
      m_comboList.Add(new ComboPair(CardBase.CardType.ATTACK, CardBase.CardType.SPECIAL),
        new BuffCombo("Blind + Atk Up", new List<Buff> { blind, atkUp }));
    }

    public static void Combo(ref Stats source, ref Stats target, int firstCardIndex)
    {
      CardBase.CardID card1 = source.m_deckMngr.m_queue[firstCardIndex].Item1,
        card2 = source.m_deckMngr.m_queue[firstCardIndex + 1].Item1;
      ComboPair pair = new ComboPair(CardManager.Get(card1).Type, CardManager.Get(card2).Type);
      Combo combo;
      if (!m_comboList.TryGetValue(pair, out combo))
      {
        Console.WriteLine("Combo does not exist! " + card1.ToString() + " + " + card2.ToString());
        return;
      }

      Console.WriteLine("Applied Combo to " + source.m_type.ToString() + ": " + combo.GetName());
      combo.ApplyEffect(ref source, ref target);
      //////Console.WriteLine($"Number of cards in queue: {source.m_deckMngr.m_queue.Length}");
      ////Console.WriteLine($"Card types in queue:");
      //foreach (var CardID in source.m_deckMngr.m_queue)
      //{
      //  //Console.WriteLine($"{CardManager.Get(CardID).Type.ToString()}");
      //}
      //first pair
      //if (source.m_deckMngr.m_queue.Length >= 2)
      //{
      //  int secondCardIndex = (firstCardIndex == 0) ? 1 : 2;
      //  if (firstCardIndex == 0)
      //    //Console.WriteLine("Resolve first combo for entity: " + source.entityID);
      //  else
      //    //Console.WriteLine("Resolve second combo for entity: " + source.entityID);

      //  switch (CardManager.Get(source.m_deckMngr.m_queue[firstCardIndex].Item1).Type)
      //  {
      //    case CardBase.CardType.ATTACK:
      //      switch (CardManager.Get(source.m_deckMngr.m_queue[secondCardIndex].Item1).Type)
      //      {
      //        case CardBase.CardType.ATTACK:
      //          source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 0, "Atk Up"));
      //          ////Console.WriteLine("Combo: Atk & Atk");
      //          break;

      //        case CardBase.CardType.BLOCK:
      //          source.m_deckMngr.Draw();
      //          ////Console.WriteLine("Combo: Atk & Block");
      //          break;

      //        case CardBase.CardType.SPECIAL:
      //          source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 0, "Atk Up"));

      //          Random rng = new Random();
      //          int chance = rng.Next(1, 2);
      //          if (chance == 1)
      //          {
      //            target.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 0, "Blinded"));
      //          }
      //          ////Console.WriteLine("Combo: Atk & Special");
      //          break;

      //        case CardBase.CardType.BLANK_CARD:
      //          break;

      //        default:
      //          break;
      //      }
      //      break;

      //    case CardBase.CardType.BLOCK:
      //      switch (CardManager.Get(source.m_deckMngr.m_queue[secondCardIndex].Item1).Type)
      //      {
      //        case CardBase.CardType.ATTACK:
      //          source.m_deckMngr.Draw();
      //          ////Console.WriteLine("Combo: Block & Atk");
      //          break;

      //        case CardBase.CardType.BLOCK:
      //          source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_BLOCK, 1.0f, 0, "Block Up"));
      //          ////Console.WriteLine("Combo: Block & Block");
      //          break;

      //        case CardBase.CardType.SPECIAL:
      //          Random rng = new Random();
      //          int chance = rng.Next(1, 2);
      //          if (chance == 1)
      //          {
      //            ////Console.WriteLine("Combo enemy is skipped");
      //            target.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 0.0f, 0, "Skipped"));
      //          }
      //          ////Console.WriteLine("Combo: Block & Special");
      //          break;

      //        case CardBase.CardType.BLANK_CARD:
      //          break;

      //        default:
      //          break;
      //      }
      //      break;

      //    case CardBase.CardType.SPECIAL:
      //      switch (CardManager.Get(source.m_deckMngr.m_queue[secondCardIndex].Item1).Type)
      //      {
      //        case CardBase.CardType.ATTACK:
      //          source.m_buffs.AddBuff(new Buff(Buff.BuffType.INCREASE_ATK_DEALT, 1.0f, 0, "Atk Up"));

      //          Random rng1 = new Random();
      //          int chance1 = rng1.Next(1, 2);
      //          if (chance1 == 1)
      //          {
      //            target.m_buffs.AddBuff(new Buff(Buff.BuffType.BLIND, 1.0f, 0, "Blinded"));
      //          }
      //          ////Console.WriteLine("Combo: Special & Atk");
      //          break;

      //        case CardBase.CardType.BLOCK:
      //          Random rng2 = new Random();
      //          int chance2 = rng2.Next(1, 2);
      //          if (chance2 == 1)
      //          {
      //            target.m_buffs.AddBuff(new Buff(Buff.BuffType.SKIP_TURN, 1.0f, 0, "Skipped"));
      //          }
      //          ////Console.WriteLine("Combo: Special & Block");
      //          break;

      //        case CardBase.CardType.BLANK_CARD:
      //          break;

      //        default:
      //          break;
      //      }
      //      break;

      //    case CardBase.CardType.BLANK_CARD:
      //      break;

      //    default:
      //      break;
      //  }

      //  //second pair
      //}

      //else
      //  //Console.WriteLine("Dont need to resolve Combo for entity: " + source.entityID);
    }
  }
}
