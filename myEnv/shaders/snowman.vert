#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform bool islight;
uniform vec4 plane;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec2 lightBias;

out vec3 normalOutput;
out vec3 posOutput;

void main()
{
    gl_ClipDistance[0] = dot(vec4(position,1.0), plane);
    gl_Position = projection * view * model * vec4(position, 1.0);
    normalOutput = mat3(transpose(inverse(model))) * normal;
    posOutput = vec3(model * vec4(position, 1.0f));
}
