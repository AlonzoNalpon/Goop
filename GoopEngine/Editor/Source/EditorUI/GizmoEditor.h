#ifndef GIZMO_EDITOR_H
#define GIZMO_EDITOR_H
#include <ImGuizmo_1_83/ImGuizmo.h>
#include <Graphics/Renderer/FrameBufferInfo.h>

namespace GE::EditorGUI
{
  class GizmoEditor
  {
  private:
    static ImGuizmo::OPERATION m_currOp;  //!< current gizmo operation
    static ImGuizmo::MODE      m_currMode;//!< current gizmo transform mode(world or local)
  public:
    struct GizmoInfo
    {
      float trans[16];
      f32 const* proj;
      f32 const* view;
      GizmoInfo(GE::Math::dMat4 const& dTrans, Graphics::Rendering::FrameBufferInfo const& framebuffer);
      GizmoInfo(float const* fTrans, Graphics::Rendering::FrameBufferInfo const& framebuffer);
      GizmoInfo();
    };
    static void UpdateGizmo(GizmoInfo& gizmoInfo);
    static void RenderGizmo();

    // static variables
    static GizmoInfo g_gizmoInfo;
  };
}

#endif
