#version 330

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_indicators;

out vec4 pass_clipSpaceReal;
out vec2 textureCoords;

uniform float height;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const float tiling = 1.0;

void main(void) {
  vec3 currentVertex = vec3(in_position.x, height, in_position.y);

  pass_clipSpaceReal = projection * view * model * vec4(currentVertex, 1.0);
  textureCoords = vec2(in_position.x/2.0 + 0.5, in_position.y/2.0 + 0.5) * tiling;
	gl_Position = pass_clipSpaceReal;
}
