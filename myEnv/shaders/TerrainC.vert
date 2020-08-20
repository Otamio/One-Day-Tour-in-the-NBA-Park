#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_color;

out vec3 pass_color;//The "flat" qualifier stops the colour from being interpolated over the triangles.
out vec3 position;
out vec3 normal;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec2 lightBias;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec4 plane;

//simple diffuse lighting
vec3 calculateLighting(){
  vec3 normal = in_normal.xyz * 2.0 - 1.0;//required just because of the format the normals were stored in (0 - 1)
  float brightness = max(dot(-lightDirection, normal), 0.0);
  return (lightColour * lightBias.x) + (brightness * lightColour * lightBias.y);
}

void main(void){

  vec4 worldPosition = model * vec4(in_position, 1.0);
  gl_ClipDistance[0] = dot(worldPosition, plane);
  gl_Position = projection * view * worldPosition;
  position = worldPosition.xyz;
  normal = in_normal;
  pass_color = in_color.rgb;
}
