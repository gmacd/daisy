#pragma once

#include "core.h"

namespace s7 {

	class Runner
	{
	public:
		Runner();
		void Init();
		void Run();
		void Shutdown();

	private:
		uint32_t _width;
		uint32_t _height;
	};

}
