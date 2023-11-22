/*!*********************************************************************
\file   SerializeComponents.cpp
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Template functions to assign data into the components of the entity.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "SerializeComponents.h"
// #include <Systems/Button/ButtonTypes.h>

namespace GE
{
	namespace ObjectFactory
	{
#ifndef RTTR_DESERIALIZE
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
			Serialization::ComponentWrapper const cw{ componentData };

			//read map, manipulate into trans, return
			GE::Component::Transform trans;
			trans.m_worldPos = cw.Get<Math::dVec3>("worldPos");
			trans.m_worldScale = cw.Get<Math::dVec3>("worldScale");
			trans.m_worldRot = cw.Get<Math::dVec3>("worldRot");

			return trans;
		}

		template <>
		GE::Component::BoxCollider DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };

			Component::BoxCollider box;
			box.m_width = cw.Get<double>("width");
			box.m_height = cw.Get<double>("height");
			box.m_center = cw.Get<Math::dVec2>("center");
#ifndef NO_IMGUI
			box.m_render = cw.Get<bool>("render");
#endif

			return box;
		}

		template<>
		GE::Component::Velocity DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };

			GE::Component::Velocity vel;
			vel.m_vel = cw.Get<Math::dVec3>("vel");
			vel.m_acc = cw.Get<Math::dVec3>("acc");
			vel.m_mass = cw.Get<double>("mass");
			vel.m_gravity = cw.Get<Math::dVec3>("gravity");
			vel.m_dragForce = cw.Get<Component::DragForce>("dragForce");
			vel.m_forces = cw.Get<std::vector<Component::LinearForce>>("forces");

			return vel;
		}

		template<>
		GE::Component::Sprite DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };
			auto& gEngine = Graphics::GraphicsEngine::GetInstance();
			auto& am = GE::Assets::AssetManager::GetInstance();

			try {
				am.GetData(cw.Get<std::string>("spriteName"));
			}
			catch (GE::Debug::IExceptionBase&)
			{
				am.LoadImageW(cw.Get<std::string>("spriteName"));
			}
			
			return Component::Sprite(gEngine.textureManager.GetTextureID(cw.Get<std::string>("spriteName")));
		}

		template<>
		GE::Component::SpriteAnim DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };
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

			spriteAnim.m_animID = gEngine.animManager.GetAnimID(cw.Get<std::string>("name"));
			return spriteAnim;
		}

		// WIP
		template<>
		GE::Component::Tween DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };
			Component::Tween tween;
			tween.m_tweens = cw.Get<std::deque<GE::Component::Tween::Action>>("tweens");
			tween.m_timeTaken = cw.Get<double>("timeTaken");
			tween.m_timeElapsed = cw.Get<double>("timeElapsed");
			tween.m_originalPos = cw.Get<Math::dVec3>("originalPos");
			tween.m_started = cw.Get<bool>("started");
			tween.m_paused = cw.Get<bool>("paused");
			
			return tween;
		}

		GE::Component::Scripts DeserializeScripts(std::string const& componentData, GE::ECS::Entity entityID)
		{
			Serialization::ComponentWrapper const cw{ componentData };
			std::vector<std::string> const vec{
				cw.Get<std::vector<std::string>>("scriptMap")
			};
			return Component::Scripts(vec, entityID);
		}

		template<>
		GE::Component::EnemyAI DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };
			return Component::EnemyAI(cw.Get<unsigned int>("treeID"));
		}


		template<>
		GE::Component::Text DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };
			Component::Text txt;
			txt.m_text = cw.Get<std::string>("text");
			txt.m_clr = cw.Get<Graphics::Colorf>("clr");
			txt.m_scale = cw.Get<f32>("scale");
			txt.m_fontID = cw.Get<Graphics::gObjID>("fontID");
			return txt;
		}

		template<>
		GE::Component::Audio DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };
			Component::Audio audio;
			audio.m_name = cw.Get<std::string>("name");
			audio.m_loop = cw.Get<bool>("loop");
			audio.m_playOnStart = cw.Get<bool>("playOnStart");
			
			return audio;
		}

		template<>
		GE::Component::GE_Button DeserializeComponent(std::string const& componentData)
		{
			Serialization::ComponentWrapper const cw{ componentData };
			Component::GE_Button button;
			button.m_param = cw.Get<std::string>("param");

			return button;
		}

#endif
	}	// namespace ObjectFactory
}	// namespace GE