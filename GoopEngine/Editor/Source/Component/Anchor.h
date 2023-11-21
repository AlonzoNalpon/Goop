/*!*********************************************************************
\file   Anchor.h
\author w.chinkitbryan\@digipen.edu
\date   21-November-2023
\brief  
  Component indicating it should be used as or to an achor.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <map>
#include <string>
#include <vector>

namespace GE::Component
{
	struct Anchor
	{
		enum ANCHOR_TYPE
		{
			IS_ANCHOR,
			IS_ANCHORABLE,
			TOTAL_TYPES
		}m_type{IS_ANCHOR};

		std::vector<GE::ECS::Entity> m_anchored;

		inline static std::string toString(ANCHOR_TYPE type)
		{
			std::string ret;
			switch (type)
			{
			case GE::Component::Anchor::IS_ANCHOR:
				ret = "isAnchor";
				break;
			case GE::Component::Anchor::IS_ANCHORABLE:
				ret = "isAnchorable";
				break;
			case GE::Component::Anchor::TOTAL_TYPES:
			default:
				break;
			}
			return ret;
		}
		inline static ANCHOR_TYPE toType(std::string type)
		{
			ANCHOR_TYPE ret{IS_ANCHOR};
			if (type == "isAnchor")
			{
				ret = IS_ANCHOR;
			}
			else if (type == "isAnchorable")
			{
				ret = IS_ANCHORABLE;
			}

			return ret;
		}

	};

	const std::map<std::string, Anchor::ANCHOR_TYPE> m_stringToAnchorType
	{
		{"isAnchor", Anchor::IS_ANCHOR},
		{"isAnchorable", Anchor::IS_ANCHORABLE}
	};
}
