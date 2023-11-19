#pragma once
#include <rttr/rttr_enable.h>

namespace GE
{
	namespace CMD
	{
		class ICommand
		{
			public:
				void virtual Execute() = 0;
				void virtual Undo() = 0;
				void virtual Redo() = 0;

				RTTR_ENABLE();
		};

	}
}
