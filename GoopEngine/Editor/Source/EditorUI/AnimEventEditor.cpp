#include <pch.h>

#ifndef IMGUI_DISABLE
#include <ImGui/imgui.h>
#include <Graphics/GraphicsEngine.h>
#include <EditorUI/AnimEventEditor.h>
#include <Math/GEM.h>
namespace GE::EditorGUI
{
  Graphics::gObjID AnimEventEditor::currAnimationEvents{Graphics::BAD_OBJ_ID};
  std::string      AnimEventEditor::currAnimationName{};

  void AnimEventEditor::CreateContent()
  {
    using namespace ImGui;
    auto& animManager = Graphics::GraphicsEngine::GetInstance().GetAnimManager();
    /*float contentSize = GetWindowSize().x;
    BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
    TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);
    TableNextColumn();
    EndTable();*/

    //Print currently editing anim name
    BeginChild("Animation Events");
    {
      TextColored({ 1.f, 1.f, 1.f, 1.f }, "Current Animation");
      SameLine();
      std::string animName = animManager.GetAnimName(currAnimationEvents);
      if (animName.empty())
        TextColored({ 1.f, 0.f, 0.f, 1.f }, "No Animation Selected!");
      else
        TextColored({ 1.f, 0.f, 0.f, 1.f }, animName.c_str());

      //if (BeginCombo("Sprite Anim", animManager.GetAnimName(spriteAnimObj->animID).c_str()))
    }
    Separator();

    // Timeline viewer
    TextColored({ 1.f, 1.f, 1.f, 1.f }, "YOUR TIMELINE HERE");
    SameLine();
    Separator();

    // The list of events IN ORDER

    EndChild();
  }
}
#endif
