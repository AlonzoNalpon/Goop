#include <pch.h>
#include "GameSystem.h"
#include <Component/Game.h>
#include <Component/Scripts.h>

using namespace GE::Component;
using namespace GE::ECS;
using namespace GE::MONO;

void GE::Systems::GameSystem::Update()
{
  static GE::FPS::FrameRateController& frc = GE::FPS::FrameRateController::GetInstance();
  //std::cout << "update Gamesystme\n";
  std::set<GE::ECS::Entity> listoFEnt{ GetUpdatableEntities() };
  //std::cout << "SIZE:" << listoFEnt.size() << "\n";
  for (Entity entity : GetUpdatableEntities())
  {
    //std::cout << "b4\n";
    Game* game = m_ecs->GetComponent<Game>(entity);
 
    MonoMethod* onUpdateFunc = mono_class_get_method_from_name(game->m_gameSystemScript.m_scriptClassInfo.m_scriptClass, "OnUpdate", 3);
    Scripts* playerScript = m_ecs->GetComponent<Scripts>(game->player);
    Scripts* enemyScript = m_ecs->GetComponent<Scripts>(game->enemy);
    auto it = playerScript->m_scriptMap.find("Stats");
    auto it2 = enemyScript->m_scriptMap.find("Stats");
    double dt = frc.GetDeltaTime();
    void* args[] = {&dt, it->second.m_classInst,it2->second.m_classInst };
    mono_runtime_invoke(onUpdateFunc, game->m_gameSystemScript.m_classInst, args, nullptr);
  


   // std::cout << "Aft\n";
    //Entity player = game->player;
    //Entity enemy = game->enemy;


    //MonoClass* gameSystemScriptObj = mono_object_get_class(game->gameSystemScript.m_classObjInst);
    //MonoMethod* update = mono_class_get_method_from_name(gameSystemScriptObj, "Update", 3);
    //double dt = frc.GetDeltaTime();
    //std::vector<void*> args{ &game->playerScript.m_classObjInst, & game->enemyScript.m_classObjInst, &dt };
    //mono_runtime_invoke(update, gameSystemScriptObj, args.data(), nullptr);
    

  }
}
