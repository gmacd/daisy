#pragma once

#include <map>
#include <string>

namespace daisy
{
	class Shader
	{
	public:
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
