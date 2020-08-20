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
    vec4 worldPosition = model * vec4(position, 1.0);
    gl_ClipDistance[0] = dot(worldPosition, plane);
    gl_Position = projection * view * worldPosition;
    normalOutput = mat3(transpose(inverse(model))) * normal;
    posOutput = worldPosition.xyz;
}
