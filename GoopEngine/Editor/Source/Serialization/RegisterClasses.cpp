#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>

RTTR_REGISTRATION
{
  using namespace rttr;
  
  registration::class_<GE::Math::dVec3>("dVec3")
               .constructor<>()
               .constructor<double, double, double>()
               .constructor<GE::Math::dVec3 const&>()
               .property("x", &GE::Math::dVec3::x)
               .property("y", &GE::Math::dVec3::y)
               .property("z", &GE::Math::dVec3::z) 
               .method("ToString", &GE::Math::dVec3::ToString)
               ;

  registration::class_<GE::Component::Transform>("Transform")
               .constructor<GE::Math::dVec3 const&, GE::Math::dVec3 const&, GE::Math::dVec3 const&>()
               .property("pos", &GE::Component::Transform::m_pos)
               .property("scale", &GE::Component::Transform::m_scale)
               .property("rot", &GE::Component::Transform::m_rot)
               ;

}
