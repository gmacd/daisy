#pragma once

#include "Shaders.h"

struct GLFWwindow;

namespace daisy
{
	class Screen
	{
	public:
		static bool ViewportChanged;

		Screen(GLFWwindow* window);

		void Update();

		float Width() const { return _width; }
		float Height() const { return _height; }
		float AspectRatio() const { return _aspectRatio; }

	private:
		GLFWwindow* _window;

	    float _width, _height, _aspectRatio;
	};


	class Context
	{
	public:
		Context(
			class Screen& screen,
			class Shaders& shaders);

	    void NewFrame();

	    double TotalElapsedTime() const { return _lastTotalTime; }
	    double LastUpdateDuration() const { return _lastUpdateDuration; }

	    Screen& Screen() const { return _screen; }
	    Shaders& Shaders() const { return _shaders; }

	private:
	    unsigned long _frameId = 0;

	    double _lastTotalTime;
	    double _lastUpdateDuration;

	    class Screen& _screen;
	    class Shaders& _shaders;
	};
}
