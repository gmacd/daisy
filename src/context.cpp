#include "context.h"

#include <GLFW/glfw3.h>

namespace daisy
{
	// Set to true to trigger recalc of screen details at beginning of nect frame
	bool Screen::ViewportChanged = true;

	Screen::Screen(GLFWwindow* window):
		_window(window)
	{
	}

	// Update the screen details (viewport, etc.) if necessary.
	void Screen::Update()
	{
		if (ViewportChanged)
		{
			ViewportChanged = false;

	        int width, height;
	        glfwGetFramebufferSize(_window, &width, &height);

	        _width = width;
	        _height = height;
	        _aspectRatio = _width / _height;

		    glViewport(0, 0, width, height);
		}
	}


	///////////////////////////////////////////////////////////////////////////

	Context::Context(
		class Screen& screen,
		class Shaders& shaders):
		_screen(screen),
		_shaders(shaders)
	{
	}

    void Context::NewFrame()
	{
		_frameId++;

		auto totalTime = glfwGetTime();
		_lastUpdateDuration = totalTime - _lastTotalTime;
		_lastTotalTime = totalTime;

		_screen.Update();
	}
}
