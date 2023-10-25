/*!*********************************************************************
\file   SerializeComponents.cpp
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Template functions to assign data into the components of the entity.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Serialization/ComponentWrapper/ComponentWrapper.h>
#include "SerializeComponents.h"

namespace GE
{
	namespace ObjectFactory
	{

		template<typename CompType>
		CompType DeserializeComponent(std::string const& componentData)
		{
			std::string str{ "Trying to deserialize unknown component type" };
			str += typeid(CompType).name();
			throw GE::Debug::Exception<CompType>(Debug::LEVEL_ERROR, ErrMsg(str));
			return CompType();
		}

		template<>
		GE::Component::Transform DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentDWrapper const cw{ componentData };

			//read map, manipulate into trans, return
			GE::Component::Transform trans;
			trans.m_pos = cw.Get<Math::dVec3>("m_pos");
			trans.m_scale = cw.Get<Math::dVec3>("m_scale");
			trans.m_rot = cw.Get<Math::dVec3>("m_rot");

			return trans;
		}

		template <>
		GE::Component::BoxCollider DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentDWrapper const cw{ componentData };

			Component::BoxCollider box;
			box.m_width = cw.Get<double>("m_width");
			box.m_height = cw.Get<double>("m_height");
			box.m_min = cw.Get<Math::dVec2>("m_min");
			box.m_max = cw.Get<Math::dVec2>("m_max");
			box.m_center = cw.Get<Math::dVec2>("m_center");
			box.m_mouseCollided = cw.Get<bool>("m_mouseCollided");

			// may not need to deserialize std::set<BoxCollider*> m_collided
			return box;
		}

		template<>
		GE::Component::Velocity DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentDWrapper const cw{ componentData };

			GE::Component::Velocity vel;
			vel.m_vel = cw.Get<Math::dVec2>("m_vel");
			vel.m_acc = cw.Get<Math::dVec2>("m_acc");
			vel.m_mass = cw.Get<double>("m_mass");
			vel.m_gravity = cw.Get<Math::dVec2>("m_gravity");
			vel.m_dragForce = cw.Get<Component::DragForce>("m_dragForce");
			vel.m_forces = cw.Get<std::vector<Component::LinearForce>>("m_dragForce");

			return vel;
		}

		template<>
		GE::Component::Sprite DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentDWrapper const cw{ componentData };
			auto& gEngine = Graphics::GraphicsEngine::GetInstance();
			auto& am = GE::Assets::AssetManager::GetInstance();
			Component::Sprite sprite;

			try {
				am.GetData(cw.Get<std::string>("filename"));
			}
			catch (GE::Debug::IExceptionBase&)
			{
				int id = am.LoadImageW(cw.Get<std::string>("filename"));
				gEngine.InitTexture(cw.Get<std::string>("filename"), am.GetData(id));
			}
			sprite.spriteData.texture = gEngine.textureManager.GetTextureID(cw.Get<std::string>("filename"));
			return sprite;
		}

		template<>
		GE::Component::SpriteAnim DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentDWrapper const cw{ componentData };
			auto& gEngine = Graphics::GraphicsEngine::GetInstance();
			// auto& am = GE::Assets::AssetManager::GetInstance();
			Component::SpriteAnim spriteAnim;

			/*try {
				am.GetData(cw.Get<std::string>("name"));
			}
			catch (GE::Debug::IExceptionBase& e)
			{
				int id = am.LoadImageW(cw.Get<std::string>("filename"));
				gEngine.InitTexture(cw.Get<std::string>("filename"), am.GetData(id));
			}*/

			spriteAnim.animID = gEngine.animManager.GetAnimID(cw.Get<std::string>("name"));
			return spriteAnim;
		}

		template<>
		GE::Component::Model DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentDWrapper const cw{ componentData };
			Component::Model model;
			model.mdlID = cw.Get<size_t>("id");

			return model;
		}

		// WIP
		template<>
		GE::Component::Tween DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentDWrapper const cw{ componentData };
			Component::Tween tween{ 0 };
			tween.m_tweens = cw.Get<std::queue<Math::dVec3>>("m_tweens");
			tween.m_timePerTween = cw.Get<double>("m_timePerTween");
			tween.m_timeTaken = cw.Get<double>("m_timeTaken");
			tween.m_timeElapsed = cw.Get<double>("m_timeElapsed");
			tween.m_originalPos = cw.Get<Math::dVec3>("m_originalPos");
			tween.m_started = cw.Get<bool>("m_started");
			
			return tween;
		}

		template<>
		GE::Component::ScriptHandler DeserializeComponent(std::string const& componentData)
		{
		    Serialization::ComponentDWrapper const cw{ componentData };
				std::vector<std::pair<std::string, std::string>> const vec{
					cw.Get<std::vector<std::pair<std::string,std::string>>>("scripts")
				};
				ECS::Entity current{ static_cast<ECS::Entity>(ECS::EntityComponentSystem::GetInstance().GetEntities().size()) };
				if (current > 0) { --current; }
		    return Component::ScriptHandler(vec, current);
		}

		// Base template to fall back on
		template <typename CompType>
		void SerializeComponent(CompType const& component, Serialization::ComponentSWrapper& wrapper)
		{
			std::ostringstream oss{};
			oss << "Trying to Serialize unsupported component: " << typeid(CompType).name();
			GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
		}

		template<>
		void SerializeComponent(GE::Component::Transform const& component, Serialization::ComponentSWrapper& wrapper)
		{
			wrapper.InsertBasicType("m_pos", component.m_pos);
			wrapper.InsertBasicType("m_scale", component.m_scale);
			wrapper.InsertBasicType("m_rot", component.m_rot);
		}	

	}	// namespace ObjectFactory
}	// namespace GE