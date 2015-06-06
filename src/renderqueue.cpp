#include "renderqueue.h"

#include <OpenGL/GL.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

namespace daisy
{

void RenderQueue::NewFrame()
{
	_cmds.clear();
}

void RenderQueue::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(_proj));

	for (auto& cmd: _cmds)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(cmd.transform));
	}
}

void RenderQueue::SetOrthoProj(const Screen& screen)
{
	_proj = glm::ortho(-screen.AspectRatio(), screen.AspectRatio(), -1.f, 1.f, 1.f, -1.f);
}

void RenderQueue::AddMesh(const glm::mat4& transform)
{
	_cmds.push_back(Mesh{transform});
}

}
