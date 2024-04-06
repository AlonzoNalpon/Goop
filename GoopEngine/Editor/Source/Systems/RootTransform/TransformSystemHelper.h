/*!*********************************************************************
\file   TransformSystemHelper.h
\author w.chinkitbryan\@digipen.edu
\date   29-October-2023
\brief  Contains definitions of helper functions for transform system.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Math/GEM.h>

namespace GE::Math
{
	/*!*********************************************************************
	\brief
	  Multiplies 2 vec3 together on a component basis.

	\param lhs
		Left hand operand

	\param rhs
	  Right hand operand

	\return
	  Result vector
	************************************************************************/
	inline dVec3 operator*(const dVec3& lhs, const dVec3& rhs)
	{
		return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
	}

	/*!*********************************************************************
	\brief
		Multiplies 2 vec3 together on a component basis.

	\param lhs
		Left hand operand

	\param rhs
		Right hand operand

	\return
		Result vector
	************************************************************************/
	inline dVec3 operator/(const dVec3& lhs, const dVec3& rhs)
	{
		return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
	}

	/*!*********************************************************************
	\brief
	  Wraps a vec3 around a range min and max.

	\param vec
		Vec to wrap

	\param min
		Min value to wrap around

	\param max
		Max value to wrap around

	\return
	  Wrapped vector
	************************************************************************/
	inline dVec3 Wrap(const dVec3& vec, double min, double max)
	{
		dVec3 ret{vec};
		while (ret.x > max) ret.x -= max;
		while (ret.x < min) ret.x += max;
		while (ret.y > max) ret.y -= max;
		while (ret.y < min) ret.y += max;
		while (ret.z > max) ret.z -= max;
		while (ret.z < min) ret.z += max;

		return ret;
	}
}
