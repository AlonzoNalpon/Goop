/*!*********************************************************************
\file   TextRenderSystem.cpp
\author a.nalpon\@digipen.edu
\date   3-November-2023
\brief  This file contains the implementation of the text render system
        for ECS. Will use renderer to perform draw requests
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Systems/Text/TextRenderSystem.h>
#include <Graphics/GraphicsEngine.h>
#include <Component/Transform.h>
namespace GE::Systems
{
  void TextRenderSystem::LateUpdate()
  {
    auto& gEngine{ Graphics::GraphicsEngine::GetInstance() };
    auto const& fontMan{ gEngine.fontManager };
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      if (!m_ecs->HasComponent<Component::Text>(entity))
        continue;
      Component::Text& textComp{ *m_ecs->GetComponent<Component::Text>(entity) };

      if (textComp.m_dirty)
      {
        textComp.m_textLinesInfo.clear();
        // Get the newlines
        fontMan.GetTextLinesInfo(textComp.m_fontID, textComp.m_text, textComp.m_textLinesInfo, textComp.m_alignment);
        textComp.m_dirty = false;
      }

      Component::Transform& trans{ *m_ecs->GetComponent<Component::Transform>(entity) };
      gEngine.GetRenderer().RenderFontObject({trans.m_worldPos.x, trans.m_worldPos.y, trans.m_worldPos.z}, 
        textComp.m_scale, textComp.m_text, 
        textComp.m_textLinesInfo, textComp.m_clr, textComp.m_fontID);
    }
  }
}
