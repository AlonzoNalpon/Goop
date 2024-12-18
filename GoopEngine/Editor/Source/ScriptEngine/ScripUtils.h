/*!*********************************************************************
\file   ScriptUtils.h
\author han.q\@digipen.edu
\date   26 November 2023
\brief
	This file contains the Enums and utils function required to transfer
	data between C# and C++

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <Component/Card.h>

namespace GE {
	namespace MONO {

		enum ScriptFieldType
		{
			Void = 0,
			Bool,
			Char,
			Short,
			Int,
			Float,
			Double,
			Long,
			UShort,
			UInt,
			ULong,
			String,
			Vec2,
			Vec3,
			DVec2,
			DVec3,
			IntArr,
			FloatArr,
			DoubleArr,
			StringArr,
			UIntArr,
			QueueFT,
			DeckFT,
			DeckManagerFT,
			CharacterTypeFT,
			HealthBarFT,
			CharacterAnimsFT
		};

		/*!*********************************************************************
		\brief
			Template along with its various specializations to get the mono
			version of an array based on the type specified
		\param MonoDomain* md
			Pointer to the MonoDomain object
		\param size_t sz
			The size of the array
		************************************************************************/
		template <typename T>
		MonoArray* GetMonoArray(MonoDomain* md, size_t sz);

		template <>
		MonoArray* GetMonoArray<int>(MonoDomain* md, size_t sz);

		template <>
		MonoArray* GetMonoArray<unsigned>(MonoDomain* md, size_t sz);

		template <>
		MonoArray* GetMonoArray<float>(MonoDomain* md, size_t sz);

		template <>
		MonoArray* GetMonoArray<double>(MonoDomain* md, size_t sz);

		template <>
		MonoArray* GetMonoArray<GE::Component::Card::CardID>(MonoDomain* md, size_t sz);

	}
}