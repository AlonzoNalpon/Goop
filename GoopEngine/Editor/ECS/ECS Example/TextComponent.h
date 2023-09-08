#pragma once
#include <string>
#include "../Component/Component.h"

struct Text : public GE::ECS::Component
{
	std::string text;
};
