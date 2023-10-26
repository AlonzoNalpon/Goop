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
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::MODEL))
   ecs.RegisterComponentToSystem<GE::Component::Model, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::VELOCITY))
   ecs.RegisterComponentToSystem<GE::Component::Velocity, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::TWEEN))
   ecs.RegisterComponentToSystem<GE::Component::Tween, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::SCRIPT_HANDLER))
   ecs.RegisterComponentToSystem<GE::Component::ScriptHandler, T>();
   if (IsBitSet(sig, ECS::COMPONENT_TYPES::DRAGGABLE))
   ecs.RegisterComponentToSystem<GE::Component::Draggable, T>();
}