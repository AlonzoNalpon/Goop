/*!*********************************************************************
\file       ObjectFactory.tpp
\author     loh.j\@digipen.edu
\co-author  chengen.lau\@digipen.edu
\date       28 September 2023
\brief      Contains the definition of template functions of ObjectFactory
            singleton class. Currently only contains a function to
            register components to a system for the engine on startup.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
template <typename T>
void ObjectFactory::RegisterComponentsToSystem(std::vector<rttr::type> const& components) const
{
  ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

  for (rttr::type const& compType : components)
  {
    if (compType == rttr::type::get<Component::Transform>())
      ecs.RegisterComponentToSystem<GE::Component::Transform, T>();
    else if (compType == rttr::type::get<Component::BoxCollider>())
      ecs.RegisterComponentToSystem<GE::Component::BoxCollider, T>();
    else if (compType == rttr::type::get<Component::SpriteAnim>())
      ecs.RegisterComponentToSystem<GE::Component::SpriteAnim, T>();
    else if (compType == rttr::type::get<Component::Sprite>())
      ecs.RegisterComponentToSystem<GE::Component::Sprite, T>();
    else if (compType == rttr::type::get<Component::Velocity>())
      ecs.RegisterComponentToSystem<GE::Component::Velocity, T>();
    else if (compType == rttr::type::get<Component::Tween>())
      ecs.RegisterComponentToSystem<GE::Component::Tween, T>();
    else if (compType == rttr::type::get<Component::Scripts>())
      ecs.RegisterComponentToSystem<GE::Component::Scripts, T>();
    else if (compType == rttr::type::get<Component::EnemyAI>())
      ecs.RegisterComponentToSystem<GE::Component::EnemyAI, T>();
    else if (compType == rttr::type::get<Component::Text>())
      ecs.RegisterComponentToSystem<GE::Component::Text, T>();
    else if (compType == rttr::type::get<Component::Audio>())
      ecs.RegisterComponentToSystem<GE::Component::Audio, T>();
    else if (compType == rttr::type::get<Component::GE_Button>())
      ecs.RegisterComponentToSystem<GE::Component::GE_Button, T>();
    else if (compType == rttr::type::get<Component::Emitter>())
      ecs.RegisterComponentToSystem<GE::Component::Emitter, T>();
    else if (compType == rttr::type::get<Component::AnimEvents>())
      ecs.RegisterComponentToSystem<GE::Component::AnimEvents, T>();
    else
    {
      std::ostringstream oss{};
      oss << "Trying to register unknown component type (" << compType.get_name().to_string()
        << ") to " << rttr::type::get<T>().get_name().to_string();
      Debug::ErrorLogger::GetInstance().LogError(oss.str());
      #ifdef _DEBUG
      std::cout << oss.str() << "\n";
      #endif
    }
  }
}
