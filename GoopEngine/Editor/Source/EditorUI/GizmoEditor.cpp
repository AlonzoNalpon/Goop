#include <pch.h>
#ifndef NO_IMGUI
#include <EditorUI/GizmoEditor.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp> // For quaternion rotations
#include <gtc/type_ptr.hpp>

#include <ECS/System/SystemManager.h>
#include <ECS/System/System.h>
#include <ECS/Entity/Entity.h>
#include <Component/Transform.h>
#include <Systems/RootTransform/PreRootTransformSystem.h>
#include <Systems/RootTransform/PostRootTransformSystem.h>
#include <Commands/CommandManager.h>

namespace GE::EditorGUI
{
  ImGuizmo::OPERATION GizmoEditor::g_currOp{ ImGuizmo::OPERATION::TRANSLATE };
  ImGuizmo::MODE      GizmoEditor::g_currMode{}; 
  bool                GizmoEditor::g_isVisible{};

  GizmoEditor::GizmoInfo GizmoEditor::g_gizmoInfo;
  void GizmoEditor::UpdateGizmo(GizmoInfo & gizmoInfo)
  {
    g_gizmoInfo = gizmoInfo;
  }

  void GizmoEditor::RenderGizmo()
  {
    //ImGuizmo::DrawGrid(g_gizmoInfo.view, g_gizmoInfo.proj, identityMatrix, 100.f);

    bool oldMode = ImGuizmo::IsUsing(); // get the old isusing flag before updating imguizmo
    ImGuizmo::Manipulate(g_gizmoInfo.view, g_gizmoInfo.proj, g_currOp
      , ImGuizmo::MODE::LOCAL, g_gizmoInfo.trans);

    bool newMode = ImGuizmo::IsUsing();
    if ((oldMode != newMode) && newMode == true) // if the mode changed from not using to using
    {
      GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
      // Save the entity's transform command here
      auto* trans = ecs.GetComponent<Component::Transform>(g_gizmoInfo.entity);
      GE::CMD::PRS oldPRS{ trans->m_pos, trans->m_rot, trans->m_scale };
      GE::CMD::ChangeTransCmd newTransCmd = GE::CMD::ChangeTransCmd(oldPRS, { trans->m_pos, trans->m_rot, trans->m_scale }, g_gizmoInfo.entity);
      GE::CMD::CommandManager& cmdMan = GE::CMD::CommandManager::GetInstance();
      cmdMan.AddCommand(newTransCmd);
    }
  }

  void GizmoEditor::PostRenderUpdate()
  {
    GE::ECS::EntityComponentSystem& ecs = GE::ECS::EntityComponentSystem::GetInstance();
    auto trans = ecs.GetComponent<GE::Component::Transform>(g_gizmoInfo.entity);
    if (!trans)
      return;
    // Reassign the new transform
    if (ImGuizmo::IsUsing())
    {

      float newScale[3], newRotation[3], newTrans[3];
      ImGuizmo::DecomposeMatrixToComponents(g_gizmoInfo.trans, newTrans, newRotation, newScale);
      // Changed local transform
      trans->m_pos.x = newTrans[0];
      trans->m_pos.y = newTrans[1];
      trans->m_pos.z = newTrans[2];
      trans->m_rot.x = newRotation[0];
      trans->m_rot.y = newRotation[1];
      trans->m_rot.z = newRotation[2];
      trans->m_scale.x = newScale[0];
      trans->m_scale.y = newScale[1];
      trans->m_scale.z = newScale[2];

      GE::Systems::PostRootTransformSystem::Propergate(g_gizmoInfo.entity, trans->m_parentWorldTransform);
      // Sets world transform relative to parent.
    
      // Update world transform
      trans->m_worldPos.x = newTrans[0];
      trans->m_worldPos.y = newTrans[1];
      trans->m_worldPos.z = newTrans[2];
      trans->m_worldRot.x = newRotation[0];
      trans->m_worldRot.y = newRotation[1];
      trans->m_worldRot.z = newRotation[2];
      trans->m_worldScale.x = newScale[0];
      trans->m_worldScale.y = newScale[1];
      trans->m_worldScale.z = newScale[2];

      GE::Systems::PreRootTransformSystem::Propergate(g_gizmoInfo.entity, trans->m_parentWorldTransform);
    }
    // Sets world transform based on ... huh?! What am I doing?!
  }

  void GizmoEditor::SetVisible(bool enable)
  {
    g_isVisible = enable;
  }

  bool GizmoEditor::isVisible()
  {
    return g_isVisible;
  }

  void GizmoEditor::SetCurrentObject(GE::Math::dMat4& dTrans, GE::ECS::Entity entity)
  {
    //if (entity == g_gizmoInfo.entity)
    //  return;
    g_gizmoInfo.entity = entity;
    // Initialize transform from double mat4x4 (ROW TO COLUMN MAJOR)
    for (int i{}; i < 4; ++i)
    {
      for (int j{}; j < 4; ++j)
      {
        g_gizmoInfo.trans[j * 4 + i] = static_cast<float>(dTrans[i][j]);
      }
    }
  }

  void GizmoEditor::SetOperation(ImGuizmo::OPERATION op)
  {
    g_currOp = op;
  }

  ImGuizmo::OPERATION GizmoEditor::GetCurrOperation()
  {
    return g_currOp;
  }

  void GizmoEditor::UpdateGizmoMtx(Graphics::Rendering::FrameBufferInfo const& framebuffer)
  {
    g_gizmoInfo.view = glm::value_ptr(framebuffer.camera.GetViewMtx());
    g_gizmoInfo.proj = glm::value_ptr(framebuffer.camera.GetProjMtx());
  }

  GizmoEditor::GizmoInfo::GizmoInfo(Graphics::Rendering::FrameBufferInfo const& framebuffer)
    : entity{},
      view{glm::value_ptr(framebuffer.camera.GetViewMtx())},
      proj{ glm::value_ptr(framebuffer.camera.GetPersMtx()) }
  {}

  GizmoEditor::GizmoInfo::GizmoInfo()
    : trans{}, proj{}, view{}
  {}

}
#endif