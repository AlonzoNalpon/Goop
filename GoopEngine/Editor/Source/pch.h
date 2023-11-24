#pragma once

//#define EVENT_DEBUG

#if defined _DEBUG || defined EVENT_DEBUG
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
#include "ECS/SystemTypes.h"
#include <DebugTools/Exception/Exception.h>
#include <DebugTools/ErrorLogger/ErrorLogger.h>
#include "FrameRateController/FrameRateController.h"
#include "InputManager/InputManager.h"
#include <Graphics/Def/GraphicsTypes.h>
#include "Events/Listener.h"
#include <Events/Events.h>
#include <Events/InputEvents.h>
#include <Utilities/GoopUtils.h>
//#define NO_IMGUI

#ifdef NO_IMGUI
//#define FULLSCREEN
#endif
