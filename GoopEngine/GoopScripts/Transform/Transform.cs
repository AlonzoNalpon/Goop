/*!*********************************************************************
\file   Vec2.cs
\author han.q\@digipen.edu
\date   28 September 2023
\brief
	This file contains a simple transform struct. This struct is a copy of 
  entity's transform component. This struct is created to allow c# scripts to
  pass transform data into cpp


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace GoopScripts
{
  public struct Transform
  {

    public Vec3<double> Pos  { get; set; }
    public Vec3<double> Scale { get; set; }
    public Vec3<double> Rot { get; set; }

    /*!*********************************************************************
     \brief
       Non default constructor of Transform struct

     \params pos
      Transform's position value

     
     \params scale
      Transform's scale value

    
     \params rot
      Transform's rotation value
     ************************************************************************/
    public Transform(Vec3<double> pos, Vec3<double> scale, Vec3<double> rot)
    {
      Pos = pos;
      Scale = scale;
      Rot = rot;
    }
  }
}
