/*!*********************************************************************
\file   pch.h
\date   15-October-2023
\brief  The pre-compiled header used by the engine

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once

//#define IMGUI_DISABLE
//#define FULLSCREEN

#if defined _DEBUG
#include <iostream>
#include <iomanip>
#endif

//stdlib
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <array>
#include <sstream>
#include <fstream>
#include <queue>
#include <deque>
#include <list>
#include <stack>
#include <algorithm>

// Engine
#include "Math/GEM.h"
#include "Math/Mat3Utils.h"
#include "Math/Mat4Utils.h"
#include "ECS/System/System.h"
#include "ECS/ComponentTypes.h"
#include "ECS/EntityComponentSystem.h"
#include "ECS/ComponentTypes.h"
#include <DebugTools/Exception/Exception.h>
#include <DebugTools/ErrorLogger/ErrorLogger.h>
#include "FrameRateController/FrameRateController.h"
#include "InputManager/InputManager.h"
#include <Graphics/Def/GraphicsTypes.h>
#include "Events/Listener.h"
#include <Events/Events.h>
#include <Events/InputEvents.h>
#include <Utilities/GoopUtils.h>
