/*!*********************************************************************
\file   IAnimationEvent.cs
\date   04-February-2024
\brief  Definition of IOnDestroy interface, which is called when the
        entity is destroyed.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Mono
{
  interface IOnDestroy
  {
    void OnDestroy(uint entityid);
  }
}
