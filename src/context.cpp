#include "context.h"

#include <GLFW/glfw3.h>

namespace daisy
{
	Context::Context(
		class Screen& screen,
		class Shaders& shaders,
		class RenderQueue& renderQueue):
		_screen(screen),
		_shaders(shaders),
		_renderQueue(renderQueue)
	{
	}

    void Context::NewFrame()
	{
		_frameId++;

		auto totalTime = glfwGetTime();
		_lastUpdateDuration = totalTime - _lastTotalTime;
		_lastTotalTime = totalTime;

		_screen.Update();
		_renderQueue.NewFrame();
	}
}
