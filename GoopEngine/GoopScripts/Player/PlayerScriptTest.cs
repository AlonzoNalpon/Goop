/*!*********************************************************************
\file   PlayerScriptTest.cs
\author han.q\@digipen.edu
\date   28 September 2023
\brief
	This c# script for testing if the script can be attahced to a the player and run


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Input;
using GoopScripts.Mono;
using static GoopScripts.Mono.Utils;

namespace GoopScripts.Player
{
  public class PlayerScriptTest : MonoBehaviour
  {
    public float scriptTestFloat = 69.0f;

    /*!*********************************************************************
   \brief
     Non default constructor of Player class

   \params enityID
    ID of the owner of this scipt
   ************************************************************************/
    public PlayerScriptTest(uint entityID) : base(entityID)
    {

    }


    /*!*********************************************************************
   \brief
     Awake function for the player script. 
   ************************************************************************/
    public void Awake()
    {
      // Logic for Awake
    }


    /*!*********************************************************************
   \brief
     Start function for the player script. 
   ************************************************************************/
    public void Start()
    {
      // Logic for Start
    }

    /*!*********************************************************************
    \brief
     Update function for the player script. This function is called every frame
     if the script is attached to an entity
    ************************************************************************/
    public void Update()
    {
      Console.WriteLine("This is the second script running specifically for player\n");
    }

    /*!*********************************************************************
    \brief
     late Update function for the player script
    ************************************************************************/
    public void LateUpdate()
    {
      // Logic for lateUpdate
    }




  }
}
