/*!*********************************************************************
\file   MonoBehaviour.cs
\author han.q\@digipen.edu
\date   28 September 2023
\brief
	base class for any C# scripts that we want to use in cpp. 
  The enityId of the script owner is stored here make it easier for 
  child script to get data


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Mono
{
  public interface MonoBehaviour
  {
    //void Init();
  }
}
