#include "shaders.h"

#include <OpenGL/GL.h>
#include <imgui.h>

namespace daisy
{
	Shader::Shader(
		const std::vector<ShaderVar>& attribs,
		const std::vector<ShaderVar>& uniforms):
		_attribs(attribs),
		_uniforms(uniforms)
	{
	}
		
	void Shaders::LoadShader(
		const std::string& id,
		const std::string& vertSource,
		const std::string& fragSource)
	{
		//ImGui::LogText("LoadShader");

		auto prog = glCreateProgram();

		auto vertShader = glCreateShader(GL_VERTEX_SHADER);
		auto vertSourceCStr = vertSource.c_str();
		glShaderSource(vertShader, 1, &vertSourceCStr, nullptr);
		glCompileShader(vertShader);

		auto fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		auto fragSourceCStr = fragSource.c_str();
		glShaderSource(fragShader, 1, &fragSourceCStr, nullptr);
		glCompileShader(fragShader);

		glAttachShader(prog, vertShader);
		glAttachShader(prog, fragShader);

		glLinkProgram(prog);

		GLint numVars;
		const int nameBufLen = 255;
		GLsizei nameLen;
		GLchar nameBuf[nameBufLen];

		// Attributes
		glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &numVars);
		std::vector<ShaderVar> attribs(numVars);
		for (auto i = 0; i < numVars; i++)
		{
			auto& attrib = attribs[i];

			glGetActiveAttrib(
				prog, i, nameBufLen-1,
				&nameLen, &attrib.numElems,
				&attrib.type, nameBuf);
			attrib.name = nameBuf;

			// Get the location
			attrib.location = glGetAttribLocation(prog, nameBuf);
		}

		// Uniforms
		glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &numVars);
		std::vector<ShaderVar> uniforms(numVars);
		for (auto i = 0; i < numVars; i++)
		{
			auto& uniform = uniforms[i];

			glGetActiveAttrib(
				prog, i, nameBufLen-1,
				&nameLen, &uniform.numElems,
				&uniform.type, nameBuf);
			uniform.name = nameBuf;

			// Get the location
			uniform.location = glGetUniformLocation(prog, nameBuf);
		}

		glDetachShader(prog, vertShader);
		glDetachShader(prog, fragShader);

		_shaders.emplace(std::make_pair(id, Shader(attribs, uniforms)));
	}
}
