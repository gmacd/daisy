#pragma once

namespace daisy
{
	class Context
	{
	public:
	    void NewFrame();

	private:
	    unsigned long _frameId = 0;
	};
}
