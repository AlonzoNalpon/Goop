/*!*********************************************************************
\file       ComboManager.cs
\author     chengen.lau\@digipen.edu
\co-author  c.phua\@digipen.edu
\co-author  han.q@digipen.edu
\date       10-January-2024
\brief  
  Resolves card combos and applies the effect to the respective characters.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;
using GoopScripts.Mono;

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
        new BuffCombo("+1 Atk", new Buff(Buff.BuffType.FLAT_ATK_UP, 1.0f, 1)));

      m_comboList.Add(new ComboPair(CardBase.CardType.ATTACK, CardBase.CardType.BLOCK),
        new DrawCombo("+1 Draw"));

      m_comboList.Add(new ComboPair(CardBase.CardType.BLOCK, CardBase.CardType.BLOCK),
        new BuffCombo("+1 Block", new Buff(Buff.BuffType.INCREASE_BLOCK, 1.0f, 1)));

      m_comboList.Add(new ComboPair(CardBase.CardType.BLOCK, CardBase.CardType.SPECIAL),
        new BuffCombo("Enemy Turn Skipped", new Buff(Buff.BuffType.SKIP_TURN, 0.0f, 1), 50));

      Buff blind = new Buff(Buff.BuffType.BLIND, 1.0f, 1);
      Buff atkUp = new Buff(Buff.BuffType.FLAT_ATK_UP, 1.0f, 1);
      m_comboList.Add(new ComboPair(CardBase.CardType.ATTACK, CardBase.CardType.SPECIAL),
        new BuffCombo("Blind, +1 Atk", new List<Buff> { blind, atkUp }));
    }

    /*!*********************************************************************
		\brief
		  Triggers a combo given the source, target and the index of the first
      card of the combo in the queue. Looks up the 2 card types in the
      dictionary for a combo. If no such combo exists, does nothing.
    \param source
      The source of the combo
    \param target
      The target of the combo
    \param firstCardIndex
      Index of the first card of the combo in the queue
		************************************************************************/
    public static void Combo(ref Stats source, ref Stats target, int firstCardIndex)
    {
      CardBase.CardID card1 = source.m_deckMngr.m_queue[firstCardIndex].Item1,
        card2 = source.m_deckMngr.m_queue[firstCardIndex + 1].Item1;
      ComboPair pair = new ComboPair(CardManager.Get(card1).Type, CardManager.Get(card2).Type);
      Combo combo;
      if (!m_comboList.TryGetValue(pair, out combo))
      {
#if (DEBUG)
        //Utils.SendString("Combo does not exist! " + card1.ToString() + " + " + card2.ToString());
#endif
        return;
      }

      
      if (combo.ApplyEffect(ref source, ref target))
      {
        Mono.Utils.SetTextComponent(source.m_comboUI[firstCardIndex], combo.GetName());
        ((ComboTextUpdate)Utils.GetScriptFromID((uint)source.m_comboUI[firstCardIndex], "ComboTextUpdate")).SetActive();
      }
    }

    /*!*********************************************************************
		\brief
		  Used by the AI's simulation to trigger a combo given the source,
      target and the 2 card types. Behaves similarly to Combo(), except
      it does not deal with UI elements.
    \param source
      The source of the combo
    \param target
      The target of the combo
    \param c1
      Type of the first card in the combo
    \param c2
      Type of the second card in the combo
		************************************************************************/
    public static void PlayCombo(ref Stats source, ref Stats target, CardBase.CardType c1, CardBase.CardType c2)
    {
      ComboPair pair = new ComboPair(c1, c2);
#if PRINT_PERM
      Console.WriteLine(m_comboList[pair].GetName());
#endif
      Combo combo;
      if (!m_comboList.TryGetValue(pair, out combo))
      {
        return;
      }
        combo.FakeApplyEffect(ref source, ref target);
    }
  } 
}
