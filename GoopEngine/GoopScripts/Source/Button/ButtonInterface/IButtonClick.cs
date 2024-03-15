/*!*********************************************************************
\file   IButtonClick.cs 
\author w.chinkitbryan\@digipen.edu
\date   06 March 2024
\brief  
  Entity mouse click callback interface
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoopScripts.Button
{
	interface IButtonClick
	{
		/*!******************************************************************
		\brief
			Callback for when mouse clicks on the entity
		
		\param entity
			Entity ID of the entity triggering the callback
		********************************************************************/
		void OnClick(uint entity);
	}
}
