/*!*********************************************************************
\file   GizmoEditor.h
\author a.nalpon\@digipen.edu
\date   27-November-2023
\brief  This file contains the definition of the gizmoEditor class
GizmoEditor is responsible for ImGuizmo interactions with components
  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
    /*!*********************************************************************
    \brief
      Updates the gizmo information. DEPRECATED
    \params
      gizmoInfo
    \return
    ************************************************************************/
    static void                 UpdateGizmo(GizmoInfo& gizmoInfo);

    /*!*********************************************************************
    \brief
      Renders the gizmo. Gizmo will update its own matrix to be used
      in post render update
    \params
    \return
    ************************************************************************/
    static void                 RenderGizmo();

    /*!*********************************************************************
    \brief
      Sets resulting transformation to bound entity if gizmo is used.
    \params
    \return
    ************************************************************************/
    static void                 PostRenderUpdate();

    /*!*********************************************************************
    \brief
      Sets visibility of gizmo.
    \params
      enable
    \return
    ************************************************************************/
    static void                 SetVisible(bool enable);

    /*!*********************************************************************
    \brief
      Accessor for gizmo visibility.
    \params
    \return
      
    ************************************************************************/
    static bool                 isVisible();

    /*!*********************************************************************
    \brief
      Sets the current entity in editor, and stores its transform.
    \params
      dTrans
      entity
    \return
    ************************************************************************/
    static void                 SetCurrentObject(GE::Math::dMat4& dTrans, GE::ECS::Entity entity);
    
    /*!*********************************************************************
    \brief
      Sets the current operation of gizmo.
    \params
      op
    \return
    ************************************************************************/
    static void                 SetOperation(ImGuizmo::OPERATION op);

    /*!*********************************************************************
    \brief
      Gets the current operation of gizmo.
    \params
    \return
      
    ************************************************************************/
    static ImGuizmo::OPERATION  GetCurrOperation();

    /*!*********************************************************************
    \brief
      Updates the gizmo matrices. This includes the camera view and 
      projection matrices
    \params
      framebuffer
    \return
    ************************************************************************/
    static void                 UpdateGizmoMtx(Graphics::Rendering::FrameBufferInfo const& framebuffer);
    
    // static variables
    static GizmoInfo g_gizmoInfo; //!< global gizmo state containing transform, entity and mode
  };
}

#endif
#endif