#include <pch.h>
#include <Systems/PlayerController/PlayerControllerSystem.h>
#include <Component/SpriteAnim.h>
#include <Component/Sprite.h>
#include <Component/Transform.h>
#include <math.h>
#include <Events/InputEvents.h>
#include <Events/EventManager.h>
#include <Graphics/GraphicsEngine.h>
#include <Component/Scripts.h>

using vec3 = GE::Math::dVec3;

using namespace GE;
using namespace ECS;
using namespace Systems;
using namespace Component;

#pragma region PLAYER_TEST
constexpr double pi = 3.14159265358979323846;
// this is to show changes in animation through 2 different sprites: 
// There's no animation controller system YET
static Graphics::gObjID sharkAnimID{};	// this is the shark animation ID
static Graphics::gObjID wormAnimID{};		// this is the worm animation ID
static					 GLuint wormSpriteID{};		// this is the worm animation ID
static					 GLuint sharkSpriteID{};		// this is the worm animation ID
#pragma endregion

void PlayerControllerSystem::Awake() 
{
	m_ecs = &EntityComponentSystem::GetInstance();
}

void GE::Systems::PlayerControllerSystem::Start()
{
	System::Start();
}

void PlayerControllerSystem::FixedUpdate() 
{
	auto& frc = GE::FPS::FrameRateController::GetInstance();
	frc.StartSystemTimer();
	std::set<ECS::Entity> ents = GetUpdatableEntities();


	for (Entity entity : GetUpdatableEntities()) {

		Scripts* scriptHan = m_ecs->GetComponent<Scripts>(entity);
		if (scriptHan != nullptr)
		{
			scriptHan->UpdateAllScripts();
		}
	}
	
	frc.EndSystemTimer("Player Controller");
}

vec3 PlayerControllerSystem::Tweening(vec3 start, vec3 end, double normalisedTime)
{
	return start + (normalisedTime * (end - start));
}

