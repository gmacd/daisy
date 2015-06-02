#include "game.h"

namespace daisy
{

void LoadGame(Context& ctx, Entities& ents)
{
	auto flatFragStr = R"x(
#version 410 core

flat in vec4 fragColour;

out vec4 outputColor;

void main()
{
	outputColor = fragColour;
}
)x";
	auto flatVertStr = R"x(
#version 410 core

in vec3 position;
in vec3 normal;
in vec3 colour;

flat out vec4 fragColour;

uniform mat4 proj, modelView, mvp;
uniform mat3 normalMat;
uniform vec3 lightPos;
uniform vec4 lightIntensity;

void main()
{
    vec3 tNorm = normalize(normalMat * normal);
    vec3 eyeCoords = vec3(modelView * vec4(position, 1.0));
    vec3 s = normalize(vec3(lightPos - eyeCoords));

    fragColour = lightIntensity * vec4(colour, 1.0) * max(dot(s, tNorm), 0.0);
    gl_Position = mvp * vec4(position, 1.0);
}
)x";

	ctx.Shaders().LoadShader("flat", flatVertStr, flatFragStr);
}
	
}
