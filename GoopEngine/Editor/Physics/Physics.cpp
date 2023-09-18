#include "Physics.h"
#include "Gravity.h"
#include "Position.h"
#include "Velocity.h"

namespace
{
	Position movement(double acceleration, Position currPos) //calculating new pos within 1 frame i.e. dt
	{
		//to test -> just print out position
	}
}

using namespace GE::Physics;
using namespace GE::Math;

virtual void PhysSys::Awake()
{
	double m_deltaTime = 0.016; //set deltatime as this first -> change to QD's dt after
	PhysSys::setDeltaTime(m_deltaTime);
}

virtual void PhysSys::Update()
{
	//update func passes curr entity
	double dt = GetDeltaTime();
	m_vel += dt * m_acc;
}