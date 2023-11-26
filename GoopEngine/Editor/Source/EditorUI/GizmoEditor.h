#ifndef GIZMO_EDITOR_H
#define GIZMO_EDITOR_H
#ifndef IMGUI_DISABLE
#include <ImGuizmo_1_83/ImGuizmo.h>
#include <Graphics/Renderer/FrameBufferInfo.h>

namespace GE::EditorGUI
{
  class GizmoEditor
  {
  private:
    static ImGuizmo::OPERATION g_currOp;  //!< current gizmo operation
    static ImGuizmo::MODE      g_currMode;//!< current gizmo transform mode(world or local)
    static bool                g_isVisible; // !< should imgui be enabled?
  public:
    struct GizmoInfo
    {
      GE::ECS::Entity entity;
      float trans[16];
      float delta[16];
      f32 const* proj;
      f32 const* view;
      GizmoInfo(Graphics::Rendering::FrameBufferInfo const& framebuffer);
      GizmoInfo();
    };
    static void                 UpdateGizmo(GizmoInfo& gizmoInfo);
    static void                 RenderGizmo();
    static void                 PostRenderUpdate();
    static void                 SetVisible(bool enable);
    static bool                 isVisible();
    static void                 SetCurrentObject(GE::Math::dMat4& dTrans, GE::ECS::Entity entity);
    static void                 SetOperation(ImGuizmo::OPERATION op);
    static ImGuizmo::OPERATION  GetCurrOperation();
    static void                 UpdateGizmoMtx(Graphics::Rendering::FrameBufferInfo const& framebuffer);
    
    // static variables
    static GizmoInfo g_gizmoInfo;
  };
}

#endif
#endif