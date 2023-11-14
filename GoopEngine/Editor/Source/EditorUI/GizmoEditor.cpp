#include <pch.h>
#include <EditorUI/GizmoEditor.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp> // For quaternion rotations
#include <gtc/type_ptr.hpp>
namespace GE::EditorGUI
{
  GizmoEditor::GizmoInfo GizmoEditor::g_gizmoInfo;
  void GizmoEditor::UpdateGizmo(GizmoInfo & gizmoInfo)
  {
    g_gizmoInfo = gizmoInfo;
  }

  void GizmoEditor::RenderGizmo()
  {
    //ImGuizmo::DrawGrid(g_gizmoInfo.view, g_gizmoInfo.proj, identityMatrix, 100.f);
    ImGuizmo::Manipulate(g_gizmoInfo.view, g_gizmoInfo.proj, ImGuizmo::OPERATION::TRANSLATE
      , ImGuizmo::MODE::LOCAL, g_gizmoInfo.trans);
  }

  GizmoEditor::GizmoInfo::GizmoInfo(GE::Math::dMat4 const& dTrans, Graphics::Rendering::FrameBufferInfo const& framebuffer)
    : view{glm::value_ptr(framebuffer.camera.GetViewMtx())},
      proj{ glm::value_ptr(framebuffer.camera.GetPersMtx()) }
  {

    // Initialize transform from double mat4x4
    for (int i{}; i < 4; ++i)
    {
      for (int j{}; j < 4; ++j)
      {
        trans[j * 4 + i] = static_cast<float>(dTrans[i][j]);
      }
    }
  }

  GizmoEditor::GizmoInfo::GizmoInfo(float const* fTrans, Graphics::Rendering::FrameBufferInfo const& framebuffer)
    : view{ glm::value_ptr(framebuffer.camera.GetViewMtx()) },
      proj{ glm::value_ptr(framebuffer.camera.GetPersMtx()) }
  {
    // Initialize transform from double mat4x4
    for (int i{}; i < 4; ++i)
    {
      for (int j{}; j < 4; ++j)
      {
        trans[j * 4 + i] = fTrans[j * 4 + i];
      }
    }
  }

  GizmoEditor::GizmoInfo::GizmoInfo()
    : trans{}, proj{}, view{}
  {}

}