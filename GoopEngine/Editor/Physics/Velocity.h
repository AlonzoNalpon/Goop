struct Velocity : public GE::ECS::Component
{
	double m_acc = 0; //starting off as stationary
	vec2 m_vel = 0;
};