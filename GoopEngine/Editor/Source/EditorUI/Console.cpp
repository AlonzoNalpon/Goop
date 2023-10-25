/*!*********************************************************************
\file   Console.cpp
\author w.chinkitbryan\@digipen.edu
\date   18-October-2023
\brief  
  Wrapper class to create an EditorGUI for the tool bar
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "Console.h"

GE::EditorGUI::Console::Console() : terminal{empty, "Console"}
{
  GE::Debug::ErrorLogger::GetInstance().AddSink(terminal.get_terminal_helper());
  GE::Debug::ErrorLogger::GetInstance().AddFileSink(terminal.get_terminal_helper());

  auto& theme = terminal.theme();
	theme.log_level_colors = {
		// Same colours as ImTerm/utils.hpp
		ImTerm::theme::constexpr_color{0.078f, 0.117f, 0.764f, 1.f}, // log_level::trace
		ImTerm::theme::constexpr_color{0.100f, 0.100f, 0.100f, 1.f}, // log_level::debug
		ImTerm::theme::constexpr_color{0.301f, 0.529f, 0.000f, 1.f}, // log_level::info
		ImTerm::theme::constexpr_color{0.784f, 0.431f, 0.058f, 1.f}, // log_level::warning
		ImTerm::theme::constexpr_color{0.901f, 0.117f, 0.117f, 1.f}, // log_level::error
		ImTerm::theme::constexpr_color{0.901f, 0.117f, 0.117f, 1.f}, // log_level::critical
	};

	terminal.log_level(ImTerm::message::severity::warn);
}

void GE::EditorGUI::Console::CreateContent()
{    
	// Static instance of the console object
	// this allows for CreateContent to be called as
	// a static obj without having to explictly create
	// a console instance.
	// There should only be 1 instance of console
	static Console cnsl;
	cnsl.terminal.show();
}
