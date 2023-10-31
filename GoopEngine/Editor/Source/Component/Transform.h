/*!*********************************************************************
\file   Transform.h
\author c.phua\@digipen.edu
\date   12 September 2023
\brief
	Component for position, scale and rotation.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Math/GEM.h>
#include "../ECS/Entity/Entity.h"
#include "../Systems/RootTransform/LocalToWorldTransform.h"
#include "../Systems/RootTransform/WorldToLocalTransfrom.h"

namespace GE
{
	namespace Component
	{
		struct Transform
		{
			using vec3 = Math::dVec3;

		private:
			vec3 m_pos;
			vec3 m_scale;
			vec3 m_rot;


		public:
			vec3 m_worldPos;
			vec3 m_worldScale;
			vec3 m_worldRot;
			/*!*********************************************************************
			\brief
				Default contructor
			************************************************************************/
			Transform() : m_pos{}, m_scale{}, m_rot{}, m_worldPos{}, m_worldScale{}, m_worldRot{} 
			{
				GE::Math::dMat4 identity
				{
					{ 1, 0, 0, 0 },
					{ 0, 1, 0, 0 },
					{ 0, 0, 1, 0 },
					{ 0, 0, 0, 1 }
				};

				m_parentWorldTransform = m_worldTransform = m_renderTransform = identity;
			}

			/*!*********************************************************************
			\brief
				Overload contructor
			************************************************************************/
			Transform(Math::dVec3 const& pos, Math::dVec3 const& scale, Math::dVec3 const& rot)
				: m_pos{ pos }, m_scale{ scale }, m_rot{ rot }, m_worldPos{ pos }, m_worldScale{ scale }, m_worldRot{ rot }
			{
				GE::Math::dMat4 identity
				{
					{ 1, 0, 0, 0 },
					{ 0, 1, 0, 0 },
					{ 0, 0, 1, 0 },
					{ 0, 0, 0, 1 }
				};

				m_parentWorldTransform = m_worldTransform = m_renderTransform = identity;
			}

			inline void SetRawPos(vec3 vec) { m_pos = vec; };
			inline void SetRawScale(vec3 vec) { m_scale = vec; };
			inline void SetRawRot(vec3 vec) { m_rot = vec; }
			inline void SetPos(GE::ECS::Entity entity, vec3 vec) { SetRawPos(vec); GE::Systems::LocalToWorldTransform::Propergate(entity, m_parentWorldTransform); };
			inline void SetScale(GE::ECS::Entity entity, vec3 vec) { SetRawScale(vec); GE::Systems::LocalToWorldTransform::Propergate(entity, m_parentWorldTransform); };
			inline void SetRot(GE::ECS::Entity entity, vec3 vec) { SetRawRot(vec); GE::Systems::LocalToWorldTransform::Propergate(entity, m_parentWorldTransform); };
			inline void SetLocalTransform(GE::ECS::Entity entity, vec3 pos, vec3 scale, vec3 rot) { m_pos = pos; m_scale = scale; m_rot = rot; GE::Systems::LocalToWorldTransform::Propergate(entity, m_parentWorldTransform); };
			inline vec3 GetPos() const { return m_pos; };
			inline vec3 GetScale() const { return m_scale; };
			inline vec3 GetRot() const { return m_rot; };

			inline void SetRawWorldPos(vec3 vec) { m_worldPos = vec; };
			inline void SetRawWorldScale(vec3 vec) { m_worldScale = vec; };
			inline void SetRawWorldRot(vec3 vec) { m_worldRot = vec; }
			inline void SetWorldPos(GE::ECS::Entity entity, vec3 vec) { SetRawWorldPos(vec); GE::Systems::WorldToLocalTransform::Propergate(entity, m_parentWorldTransform); };
			inline void SetWorldScale(GE::ECS::Entity entity, vec3 vec) { SetRawWorldScale(vec); GE::Systems::WorldToLocalTransform::Propergate(entity, m_parentWorldTransform); };
			inline void SetWorldRot(GE::ECS::Entity entity, vec3 vec) { SetRawWorldRot(vec); GE::Systems::WorldToLocalTransform::Propergate(entity, m_parentWorldTransform); };
			inline void SetWorldTransform(GE::ECS::Entity entity, vec3 pos, vec3 scale, vec3 rot) { m_pos = pos; m_scale = scale; m_rot = rot; GE::Systems::WorldToLocalTransform::Propergate(entity, m_parentWorldTransform); };
			inline vec3 GetWorldPos() const { return m_worldPos; };
			inline vec3 GetWorldScale() const { return m_worldScale; };
			inline vec3 GetWorldRot() const { return m_worldRot; };

			Math::dMat4 m_parentWorldTransform;
			Math::dMat4 m_worldTransform;
			Math::dMat4 m_renderTransform;
		};
	}
}