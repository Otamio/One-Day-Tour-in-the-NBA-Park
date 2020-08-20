#version 330 core

in vec3 pass_color;
in vec3 position;
in vec3 normal;

out vec4 out_color;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;
uniform bool normal_coloring;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec2 lightBias;
uniform float useToon;

void main(void){

	vec3 color;

	color = pass_color;

	if (useToon == 1) {
		vec3 unitLightVector = normalize(lightPos - position);
		float nDotl = dot(normal, unitLightVector);
		float brightness = max(nDotl, 0.0);

		if (brightness > 0.95)
			color = vec3(1.0,1,1) * color;
		else if (brightness > 0.5)
			color = vec3(0.7,0.7,0.7) * color;
		else if (brightness > 0.25)
			color = vec3(0.35,0.35,0.35) * color;
		else
			color = vec3(0.1,0.1,0.1) * color;
	} else if (useToon == 2) {
		vec3 viewDir = normalize(viewPos - position);
		float nDotv = dot(normal, viewDir);
		float edge = max(nDotv, 0.0);

		if (edge < 0.01) {
			color = vec3(0.0,0.0,0.0);
		}
	}

	out_color = vec4(color, 1.0);
}
