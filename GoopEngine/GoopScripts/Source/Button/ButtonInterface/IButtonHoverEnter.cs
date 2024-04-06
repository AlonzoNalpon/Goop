/*!*********************************************************************
\file   IButtonHoverEnter.cs 
\author w.chinkitbryan\@digipen.edu
\date   06-March-2024
\brief  
  Entity mouse hover enter callback interface
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
	interface IButtonHoverEnter
	{
		/*!******************************************************************
		\brief
			Callback for when mouse enters hover on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		void OnHoverEnter(uint entity);
	}
}
