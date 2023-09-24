#pragma once
#include <Window/Window.h>

namespace GE::EditorGUI
{
	class ImGuiUI
	{
	public:
		void Init(WindowSystem::Window& window);
		void Update();
		void Render();
		void Exit();
	};
}
