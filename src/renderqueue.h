#pragma once

#include <vector>

#include <glm/mat4x4.hpp>
#include "screen.h"

namespace daisy 
{
	struct Mesh
	{
		glm::mat4 transform;
	};

	class RenderQueue
	{
	public:
		void NewFrame();
		void Render();

		void SetOrthoProj(const daisy::Screen& screen);

		void AddMesh(const glm::mat4& transform);

	private:
		glm::mat4 _proj;
		std::vector<Mesh> _cmds;
	};
}
