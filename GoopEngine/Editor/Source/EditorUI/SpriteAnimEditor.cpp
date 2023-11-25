#include <pch.h>
#ifndef NO_IMGUI
#include <EditorUI/SpriteAnimEditor.h>
#include <ImGui/imgui.h>
#include <Graphics/GraphicsEngine.h>
namespace GE::EditorGUI
{
  Graphics::gObjID SpriteAnimEditor::currAnimation{};
  void SpriteAnimEditor::CreateContent()
  {
    using namespace ImGui;
    auto& animManager = Graphics::GraphicsEngine::GetInstance().GetAnimManager();
    /*float contentSize = GetWindowSize().x;
    BeginTable("##", 1, ImGuiTableFlags_BordersInnerV);
    TableSetupColumn("Col1", ImGuiTableColumnFlags_WidthFixed, contentSize);
    TableNextColumn();
    EndTable();*/

    //Print currently editing anim name
    {
      TextColored({ 1.f, 1.f, 1.f, 1.f }, "Current Animation");
      SameLine();
      std::string animName = animManager.GetAnimName(currAnimation);
      if (animName.empty())
        TextColored({ 1.f, 0.f, 0.f, 1.f }, "No Animation Selected!");
      else
        TextColored({ 1.f, 0.f, 0.f, 1.f }, animName.c_str());

      //if (BeginCombo("Sprite Anim", animManager.GetAnimName(spriteAnimObj->animID).c_str()))
    }
    Separator();
  }
}
#endif
