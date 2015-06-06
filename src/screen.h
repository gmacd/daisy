#pragma once

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
}
