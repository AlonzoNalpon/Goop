#include <pch.h>
#include "GameSystem.h"
#include <Component/Game.h>
#include <Component/Scripts.h>

using namespace GE::Component;
using namespace GE::ECS;
using namespace GE::MONO;

void GE::Systems::GameSystem::Update()
{
  //static GE::FPS::FrameRateController& frc = GE::FPS::FrameRateController::GetInstance();

  //for (Entity entity : GetUpdatableEntities())
  //{
  //  Game* game = m_ecs->GetComponent<Game>(entity);
  //  Entity player = game->player;
  //  Entity enemy = game->enemy;


  //  MonoClass* gameSystemScriptObj = mono_object_get_class(game->gameSystemScript.m_classObjInst);
  //  MonoMethod* update = mono_class_get_method_from_name(gameSystemScriptObj, "Update", 3);
  //  double dt = frc.GetDeltaTime();
  //  std::vector<void*> args{ &game->playerScript.m_classObjInst, & game->enemyScript.m_classObjInst, &dt };
  //  mono_runtime_invoke(update, gameSystemScriptObj, args.data(), nullptr);
  //}
}
