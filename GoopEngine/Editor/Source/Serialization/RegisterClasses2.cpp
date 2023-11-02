#include <pch.h>
#include <Component/Components.h>
#include <Systems/Systems.h>
#include <rttr/registration>
#include <Serialization/Serializer.h>
#include <Systems/Enemy/EnemySystem.h>

using namespace GE;

RTTR_REGISTRATION
{
	/* ------------------- MATH ------------------- */

	/* ------------------- COMPONENTS ------------------- */
		rttr::registration::class_<Component::ScriptHandler>("ScriptHandler")
		.constructor<std::vector<std::string> const&, unsigned>()
		.property("scriptMap", &Component::ScriptHandler::m_scriptMap)
	  .method("AddScript", &Component::ScriptHandler::AddScript)
	;
	/* ------------------- ENUMERATIONS ------------------- */


	/* ------------------- FUNCTIONS ------------------- */

} // RTTR Registration
