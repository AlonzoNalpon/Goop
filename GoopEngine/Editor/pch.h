#pragma once

#ifdef _DEBUG
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

// Engine
#include "Math/GEM.h"
#include "ECS/Component/Component.h"
#include "ECS/System/System.h"
#include "FrameRateController/FrameRateController.h"
#include "ECS/EntityComponentSystem.h"
#include "Debugger/Exception/Exception.h"
#include "Debugger/ErrorLogger/ErrorLogger.h"
