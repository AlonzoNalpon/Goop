#pragma once
#include <pch.h>
#include <ScriptEngine/ScripUtils.h>
#include <iostream>

namespace GE {
	namespace MONO {

	template <typename T>
	MonoArray* GetMonoArray(MonoDomain* md, uintptr_t sz) {
		return mono_array_new(md, mono_get_int32_class(), sz);
	}

	template <>
	MonoArray* GetMonoArray<int>(MonoDomain* md, uintptr_t sz) {
		return mono_array_new(md, mono_get_int32_class(), sz);
	}

	template <>
	MonoArray* GetMonoArray<unsigned>(MonoDomain* md, uintptr_t sz) {
		return mono_array_new(md, mono_get_uint32_class(), sz);
	}

	template <>
	MonoArray* GetMonoArray<float>(MonoDomain* md, uintptr_t sz) {
		return mono_array_new(md, mono_get_single_class(), sz);
	}

	template <>
	MonoArray* GetMonoArray<double>(MonoDomain* md, uintptr_t sz) {
		return mono_array_new(md, mono_get_double_class(), sz);
	}

	}
}