#pragma once

#include "RenderQueue.h"
#include "Shaders.h"
#include "Screen.h"

namespace daisy
{
	class Context
	{
	public:
		Context(
			class Screen& screen,
			class Shaders& shaders,
			class RenderQueue& renderQueue);

	    void NewFrame();

	    double TotalElapsedTime() const { return _lastTotalTime; }
	    double LastUpdateDuration() const { return _lastUpdateDuration; }

	    Screen& Screen() const { return _screen; }
	    Shaders& Shaders() const { return _shaders; }
	    RenderQueue& RenderQueue() const { return _renderQueue; }

	private:
	    unsigned long _frameId = 0;

	    double _lastTotalTime;
	    double _lastUpdateDuration;

	    class Screen& _screen;
	    class Shaders& _shaders;
	    class RenderQueue& _renderQueue;
	};
}
