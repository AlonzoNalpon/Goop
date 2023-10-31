/*!*********************************************************************
\file   Entity.h
\author w.chinkibryan\@digipen.edu
\date   9-September-2023
\brief  
  Basic defines for required for entities.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once

#include <bitset>
#include <limits>
#include <Math/GEM.h>
#include <set>
#include <memory>

namespace GE
{
	namespace ECS
	{
		constexpr size_t MAX_COMPONENTS{ 32 };
		using EntityID = unsigned int;
		constexpr EntityID INVALID_ID{ std::numeric_limits<unsigned int>::max() };

		// This is just a bitfield to track unique components
		// Functionally the same as ComponentType
		using ComponentSignature = std::bitset<MAX_COMPONENTS>;

		class Entity
		{
		private:
			using vec3 = GE::Math::dVec3;
			using mat4 = GE::Math::dMat4;

			class LocalTransform
			{
			private:
				vec3 m_vec;
				Entity& m_entity;
			public:
				LocalTransform(Entity& entity);
				operator vec3();
				operator vec3() const;
				vec3& GetRaw();
				LocalTransform& operator=(const vec3& lhs);
			};
			class WorldTransform
			{
			private:
				vec3 m_vec;
				Entity& m_entity;
			public:
				WorldTransform(Entity& entity);
				operator vec3();
				operator vec3() const;
				vec3& GetRaw();
				WorldTransform& operator=(const vec3& lhs);
			};

			std::set<Entity> m_children;
			std::shared_ptr<Entity> m_parent;
		public:
			Entity(const Entity& other);
			Entity(EntityID id, std::string name = "");

			EntityID m_id;
			bool m_active;
			std::string m_name;

			LocalTransform m_pos;
			LocalTransform m_scale;
			LocalTransform m_rot;

			WorldTransform m_worldPos;
			WorldTransform m_worldScale;
			WorldTransform m_worldRot;

			mat4 m_parentWorldTransform;
			mat4 m_worldTransform;
			mat4 m_renderTransform;

			operator EntityID();
			operator EntityID() const;
			void operator=(Entity& other);
			bool operator<(const Entity& other);
			inline void SetParent(Entity& entity);
			inline void SetParent(void* = nullptr);
			inline Entity& GetParent();
			inline const Entity& GetParent() const;
			inline void AddChildren(Entity& entity);
			inline void RemoveChildren(Entity& entity);
			inline std::set<Entity>& GetChildren();
			inline const std::set<Entity>& GetChildren() const;
		};
	}
}
