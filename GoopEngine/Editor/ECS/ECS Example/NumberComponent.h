#pragma once
#include "../Component/Component.h"

struct Number : public GE::ECS::Component
{
	int a;
	int b;
	int c;
	int total;
};