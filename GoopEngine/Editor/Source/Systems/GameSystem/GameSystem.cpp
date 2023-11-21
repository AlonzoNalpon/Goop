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
  std::cout << "update Gamesystme\n";
  std::set<GE::ECS::Entity> listoFEnt{ GetUpdatableEntities() };
  std::cout << "SIZE:" << listoFEnt.size() << "\n";
  for (Entity entity : GetUpdatableEntities())
  {
    std::cout << "b4\n";
    Game* game = m_ecs->GetComponent<Game>(entity);
    game->m_gameSystemScript.InvokeOnUpdate(frc.GetDeltaTime());

    //GE::MONO::ScriptManager* sm = &GE::MONO::ScriptManager::GetInstance();
    //m_scriptClassInfo = sm->GetScriptClassInfo(scriptName);
    //m_classInst = sm->InstantiateClass(scriptName.c_str(), arg);



    MonoMethod* m_onTest = mono_class_get_method_from_name(game->m_gameSystemScript.m_scriptClassInfo.m_scriptClass, "OnTest", 1);
    Scripts* playerScript = m_ecs->GetComponent<Scripts>(game->player);
    auto it = playerScript->m_scriptMap.find("Stats");
    if (it != playerScript->m_scriptMap.end())
    {
      MonoProperty* healthProperty = mono_class_get_property_from_name(it->second.m_scriptClassInfo.m_scriptClass, " m_health");
      int h = 100;
      std::vector<void*> par{ &h };
      mono_property_set_value(healthProperty, it->second.m_classInst, par.data(), nullptr);


      MonoObject* test = it->second.m_classInst;
      void* param = &(test);
      mono_runtime_invoke(m_onTest, game->m_gameSystemScript.m_classInst, &param, nullptr);
    }
  


    std::cout << "Aft\n";
    //Entity player = game->player;
    //Entity enemy = game->enemy;


    //MonoClass* gameSystemScriptObj = mono_object_get_class(game->gameSystemScript.m_classObjInst);
    //MonoMethod* update = mono_class_get_method_from_name(gameSystemScriptObj, "Update", 3);
    //double dt = frc.GetDeltaTime();
    //std::vector<void*> args{ &game->playerScript.m_classObjInst, & game->enemyScript.m_classObjInst, &dt };
    //mono_runtime_invoke(update, gameSystemScriptObj, args.data(), nullptr);
    

  }
}
