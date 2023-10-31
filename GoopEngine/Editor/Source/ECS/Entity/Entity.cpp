#include <pch.h>
#include "Entity.h"
#include <Systems/RootTransform/LocalToWorldTransform.h>
#include <Systems/RootTransform/WorldToLocalTransform.h>

using vec3 = GE::Math::dVec3;
using namespace GE::ECS;

// Proxy class LocalTransform defines
Entity::LocalTransform::LocalTransform(Entity& entity) : m_entity{ entity } {}
Entity::LocalTransform::operator vec3() { return m_vec; }
GE::ECS::Entity::LocalTransform::operator vec3() const { return m_vec; }
vec3& Entity::LocalTransform::GetRaw() { return m_vec; }
Entity::LocalTransform& Entity::LocalTransform::operator=(const vec3& lhs)
{
	m_vec = lhs;
	GE::Systems::LocalToWorldTransform::Propergate(m_entity, m_entity.m_parentWorldTransform);
}

// Proxy class WorldTransform defines
Entity::WorldTransform::WorldTransform(Entity& entity) : m_entity{ entity } {}
Entity::WorldTransform::operator vec3() { return m_vec; }
GE::ECS::Entity::WorldTransform::operator vec3() const { return m_vec; }
vec3& Entity::WorldTransform::GetRaw() { return m_vec; }
Entity::WorldTransform& Entity::WorldTransform::operator=(const vec3& lhs)
{
	m_vec = lhs;
	GE::Systems::WorldToLocalTransform::Propergate(m_entity, m_entity.m_parent->m_worldTransform);
}

GE::ECS::Entity::Entity(const Entity& other) : m_active{ false }, m_id{ other.m_id }, m_name{ other.m_name }, m_parent{ std::make_shared<Entity>(INVALID_ID) },
m_pos{ *this }, m_scale{ *this }, m_rot{ *this },
m_worldPos{ *this }, m_worldScale{ *this }, m_worldRot{ *this }
{
	GE::Math::dMat4 identity
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	m_parentWorldTransform = identity;
	m_worldTransform = identity;
	m_renderTransform = identity;
}

// Entity class defines
Entity::Entity(EntityID id, std::string name) : m_active{ false }, m_id{ id }, m_name{ name }, m_parent{ std::make_shared<Entity>(INVALID_ID) },
m_pos{ *this }, m_scale{ *this }, m_rot{ *this },
m_worldPos{ *this }, m_worldScale{ *this }, m_worldRot{ *this }
{
	GE::Math::dMat4 identity
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	m_parentWorldTransform = identity;
	m_worldTransform = identity;
	m_renderTransform = identity;
}
GE::ECS::Entity::operator EntityID() { return m_id; }
GE::ECS::Entity::operator EntityID() const { return m_id; }
void Entity::operator=(Entity& other)
{
	m_id = other.m_id; 
	m_pos.GetRaw() = other.m_pos.GetRaw();
	m_scale.GetRaw() = other.m_scale.GetRaw();
	m_rot.GetRaw() = other.m_rot.GetRaw();
	m_worldPos.GetRaw() = other.m_worldPos.GetRaw();
	m_worldScale.GetRaw() = other.m_worldScale.GetRaw();
	m_worldRot.GetRaw() = other.m_worldRot.GetRaw();
	m_parentWorldTransform = other.m_parentWorldTransform;
	m_worldTransform = other.m_worldTransform;
	m_renderTransform = other.m_renderTransform;
}
bool GE::ECS::Entity::operator<(const Entity& other) { return m_id < other.m_id; }
inline void Entity::SetParent(Entity& entity) { *m_parent = entity; }
inline void GE::ECS::Entity::SetParent(void*) { m_parent->m_id = INVALID_ID; }
inline Entity& Entity::GetParent() { return *(m_parent.get()); }
inline const Entity& Entity::GetParent() const { return *(m_parent.get()); }
inline void Entity::AddChildren(Entity& entity) { m_children.insert(entity); }
inline void Entity::RemoveChildren(Entity& entity) { m_children.erase(entity); }
inline std::set<Entity>& Entity::GetChildren() { return m_children; }
inline const std::set<Entity>& Entity::GetChildren() const { return m_children; }
