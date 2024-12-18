/*!*********************************************************************
\file   def.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains all typedefs for project.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef DEF_H
#define DEF_H
#include <iostream>
#include <string>
#include <gl/glew.h>
using ubyte   = unsigned char;
using uChar   = unsigned char;
using uint32  = unsigned int;
using i32     = int;
using f64     = double;
using f32     = float;
using wchar   = wchar_t; // word (2 byte) chars. 
using u32     = unsigned int;
using u64     = unsigned long long;

#pragma region CONSTANTS
std::string const ASSETS_PATH{ "./Assets/" };
std::string const SHADER_PATH{ ASSETS_PATH + "Shaders/" };
#pragma endregion

#ifndef _DEBUG // RELEASE MODE
#define NULL_CHECK_RET(x) if(!x)return     // Returns if value is null
#define NULL_CHECK_BREAK(x) if(!x)break    // breaks if value is null
#else
#define NULL_CHECK_RET(x)   // does nothing in debug. Hope to crash.
#define NULL_CHECK_BREAK(x) // does nothing in debug. Hope to crash
#endif

#ifdef _DEBUG

#define INVALID_VALUE_CHECK 
#endif
#endif