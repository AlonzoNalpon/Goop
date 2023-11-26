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
	Events::EventManager::GetInstance().Subscribe<Events::KeyHeldEvent>(this);
	Events::EventManager::GetInstance().Subscribe<Events::KeyTriggeredEvent>(this);

}

void GE::Systems::PlayerControllerSystem::Start()
{
	System::Start();
	auto& gEngine = Graphics::GraphicsEngine::GetInstance();
	sharkAnimID = gEngine.animManager.GetAnimID("SS_Shark");
	wormAnimID = gEngine.animManager.GetAnimID("SS_MineWorm");
	wormSpriteID = gEngine.textureManager.GetTextureID("SS_MineWorm");
	sharkSpriteID = gEngine.textureManager.GetTextureID("SS_Shark");
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

void PlayerControllerSystem::HandleEvent(Events::Event* event)
{
	double dt = GE::FPS::FrameRateController::GetInstance().GetDeltaTime();
	for (Entity entity : m_entities) {
		Transform* trans = m_ecs->GetComponent<Transform>(entity);
		SpriteAnim* spriteAnim = m_ecs->GetComponent<SpriteAnim>(entity);
		Sprite*	sprite = m_ecs->GetComponent<Sprite>(entity);
		if (event->GetCategory() == Events::EVENT_TYPE::KEY_HELD)
		{
			KEY_CODE const key{ static_cast<Events::KeyHeldEvent*>(event)->GetKey() };
			if (key == GPK_H)
			{
				constexpr double ROTATE_SPEED{ 2.0 };
				trans->m_rot.z = fmod(trans->m_rot.z + dt * ROTATE_SPEED, pi * 2.0); // ROTATING PLAYER
			}
			if (key == GPK_J)
			{
				constexpr double SCALE_SPEED{ 30.0 };
				constexpr double MAX_SCALE{ 200.0 };
				constexpr double MIN_SCALE{ 100.0 };
				trans->m_scale.x = fmod(trans->m_scale.x + dt * SCALE_SPEED, MAX_SCALE); // SCALING PLAYER
				trans->m_scale.y = fmod(trans->m_scale.y + dt * SCALE_SPEED, MAX_SCALE); // SCALING PLAYER
				trans->m_scale.x = fmax(trans->m_scale.x, MIN_SCALE);
				trans->m_scale.y = fmax(trans->m_scale.y, MIN_SCALE);
			}

#ifdef EVENT_DEBUG
			 << "PlayerControllerSystem: " << event->GetName() + " Event handled\n";
#endif
		}
		else if (event->GetCategory() == Events::EVENT_TYPE::KEY_TRIGGERED)
		{
			KEY_CODE const key{ static_cast<Events::KeyHeldEvent*>(event)->GetKey() };
			if (key == GPK_K)
			{
				spriteAnim->currFrame = 0;
				spriteAnim->currTime = 0.0;
				// changing sprite
				if (spriteAnim->animID == sharkAnimID)
				{
					spriteAnim->animID = wormAnimID;
					sprite->m_spriteData.texture = wormSpriteID;
				}
				else if (spriteAnim->animID == wormAnimID)
				{
					spriteAnim->animID = sharkAnimID;
					sprite->m_spriteData.texture = sharkSpriteID;
				}
			}

#ifdef EVENT_DEBUG
			 << "PlayerControllerSystem: " << event->GetName() + " Event handled\n";
#endif
		}
	}
}

vec3 PlayerControllerSystem::Tweening(vec3 start, vec3 end, double normalisedTime)
{
	return start + (normalisedTime * (end - start));
}

