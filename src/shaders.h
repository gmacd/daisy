#pragma once

#include <map>
#include <string>
#include <vector>

#include <OpenGL/GL.h>

namespace daisy
{
	struct ShaderVar
	{
		std::string name;
		GLint location;
		GLint numElems;
		GLenum type;
	};

	class Shader
	{
	public:
		Shader(
			const std::vector<ShaderVar>& attribs,
			const std::vector<ShaderVar>& uniforms);

	private:
		std::vector<ShaderVar> _attribs;
		std::vector<ShaderVar> _uniforms;
	};

	class Shaders
	{
	public:
		void LoadShader(
			const std::string& id,
			const std::string& vertSource,
			const std::string& fragSource);
	private:
		std::map<std::string, Shader> _shaders;
	};
}
