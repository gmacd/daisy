$input a_position, a_color0, a_normal
$output v_color0


#include "../../bgfx/src/bgfx_shader.sh"


uniform vec4 u_lightPos;
uniform vec4 u_lightIntensity;


void main()
{
	//gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

    // normalMat?
    vec3 tNorm = normalize(normalMat * a_normal);
    vec3 eyeCoords = u_modelView * vec4(a_position, 1.0);
    vec3 s = normalize(u_lightPos - eyeCoords);

    v_color0 = u_lightIntensity * vec4(a_color0, 1.0) * max(dot(s, tNorm), 0.0);
    gl_Position = mvp * vec4(a_position, 1.0);
}
