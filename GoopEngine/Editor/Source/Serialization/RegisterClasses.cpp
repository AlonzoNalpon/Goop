#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration.h>

RTTR_REGISTRATION
{
  using namespace rttr;
  
registration::class_<GE::Math::dVec3>("dVec3")
             .constructor<>()
             .constructor<double, double, double>()
             .constructor<GE::Math::dVec3 const&>()
             .property("x", &GE::Math::dVec3::x, registration::public_access)
             .property("y", &GE::Math::dVec3::y, registration::public_access)
             .property("z", &GE::Math::dVec3::z, registration::public_access)
             ;

registration::class_<GE::Component::Transform>("Transform")
             .constructor<GE::Math::dVec3 const&, GE::Math::dVec3 const&, GE::Math::dVec3 const&>()
             .property("pos", &GE::Component::Transform::m_pos, registration::public_access)
             .property("scale", &GE::Component::Transform::m_scale, registration::public_access)
             .property("rot", &GE::Component::Transform::m_rot, registration::public_access)
             ;

}
