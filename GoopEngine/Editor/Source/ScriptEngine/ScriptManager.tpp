template<typename T>
std::vector<T> MonoArrayToSTD(MonoArray* arr)
{ 
	std::vector<T> test{};
	for (int i = 0; i < mono_array_length(arr); ++i) {
			T element = mono_array_get(arr, T, i);
			test.push_back(element);
	}

	return test;
}