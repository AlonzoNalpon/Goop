/*!*********************************************************************
\file   Console.h
\author w.chinkitbryan\@digipen.edu
\date   18-October-2023
\brief  
  Wrapper class to create an EditorGUI for the tool bar
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include "../ImTerm/terminal_helpers.hpp"
#include "../ImTerm/terminal.hpp"

namespace GE::EditorGUI
{
  struct empty_command_struct {};

  class Terminal : public ImTerm::basic_spdlog_terminal_helper<Terminal, empty_command_struct, misc::no_mutex>
  {
  };

  class Console
  {
  private:
    empty_command_struct empty;
    ImTerm::terminal<Terminal> terminal;
  public:
    /*!******************************************************************
    \brief
      Terminal theme and spdlog setup
    ********************************************************************/
    Console();

    /*!******************************************************************
    \brief
      Wrapper to call ImTerm console
    ********************************************************************/
    static void CreateContent();
  };

}

#endif