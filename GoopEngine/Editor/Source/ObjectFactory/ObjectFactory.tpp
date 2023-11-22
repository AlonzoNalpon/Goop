template <typename T>
void ObjectFactory::RegisterComponentsToSystem(ECS::ComponentSignature sig) const
{
   ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

   if (IsBitSet(sig, ECS::COMPONENT_TYPES::TRANSFORM))
   ecs.RegisterComponentToSystem<GE::Component::Transform, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::BOX_COLLIDER))
   ecs.RegisterComponentToSystem<GE::Component::BoxCollider, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::SPRITE_ANIM))
   ecs.RegisterComponentToSystem<GE::Component::SpriteAnim, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::SPRITE))
   ecs.RegisterComponentToSystem<GE::Component::Sprite, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::VELOCITY))
   ecs.RegisterComponentToSystem<GE::Component::Velocity, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::TWEEN))
   ecs.RegisterComponentToSystem<GE::Component::Tween, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::SCRIPTS))
   ecs.RegisterComponentToSystem<GE::Component::Scripts, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::DRAGGABLE))
   ecs.RegisterComponentToSystem<GE::Component::Draggable, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::ENEMY_AI))
   ecs.RegisterComponentToSystem<GE::Component::EnemyAI, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::TEXT))
   ecs.RegisterComponentToSystem<GE::Component::Text, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::AUDIO))
   ecs.RegisterComponentToSystem<GE::Component::Audio, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::GE_BUTTON))
   ecs.RegisterComponentToSystem<GE::Component::GE_Button, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::GAME))
   ecs.RegisterComponentToSystem<GE::Component::Game, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::ANCHOR))
   ecs.RegisterComponentToSystem<GE::Component::Anchor, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::CARD))
   ecs.RegisterComponentToSystem<GE::Component::Card, T>();
}