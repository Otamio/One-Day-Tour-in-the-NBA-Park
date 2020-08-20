#version 330 core

in vec3 posOutput;
in vec3 normalOutput;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;
uniform bool normal_coloring;
uniform bool islight;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec2 lightBias;

out vec4 fragColor;

void main()
{
  vec4 color = vec4(1,1,1,1);

  fragColor = color;
}
