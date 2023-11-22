#pragma once
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

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
			Vec2,
			Vec3,
			DVec2,
			DVec3,
			IntArr,
			FloatArr,
			DoubleArr
		};

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

	}
}