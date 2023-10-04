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
  public class MonoBehaviour
  {
    protected uint m_entityID;

    /*!*********************************************************************
    \brief
      Non default constructor of Monobehavior class

    \params enityID
     ID of the owner of this scipt
    ************************************************************************/
    public MonoBehaviour(uint entityID)
    {
      m_entityID = entityID;
    }
  }
}
