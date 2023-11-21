/*!*********************************************************************
\file   Anchor.h
\author w.chinkitbryan\@digipen.edu
\date   21-November-2023
\brief  
  Component indicating it should be used as or to an achor.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once

namespace GE::Component
{
	struct Anchor
	{
		enum ANCHOR_TYPE
		{
			IS_ANCHOR,
			IS_ANCHORABLE
		}m_type;
	};
}
