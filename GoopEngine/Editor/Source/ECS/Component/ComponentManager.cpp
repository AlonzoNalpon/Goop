/*!*********************************************************************
\file   ComponentManager.cpp
\author w.chinkitbryan\@digipen.edu
\date   6-September-2023
\brief  
  Custom array data structure to contain components
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ComponentManager.h"

using namespace GE::ECS;

ComponentManager::~ComponentManager()
{
	for (auto& cmpArr : m_componentArrays)
	{
		if (cmpArr.second == nullptr)
		{
			continue;
		}
		delete[] cmpArr.second;
		cmpArr.second = nullptr;
	}
}

void ComponentManager::EntityDestroyed(Entity& entity)
{
	for (auto& cmpArr : m_componentArrays)
	{
		cmpArr.second->EntityDestroyed(entity);
	}
}