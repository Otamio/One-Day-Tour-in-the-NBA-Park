#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Pos;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, TexCoords);
    // FragColor = vec4(0.7, 0.3, 0.3, 0.5);
}
