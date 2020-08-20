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
uniform vec3 viewPos;
uniform vec4 plane;

out vec4 fragColor;

vec3 computePhong(vec3 lightColor, vec3 lightPos, vec3 posOutput, vec3 normalOutput,
                  vec3 material_ambient, vec3 material_diffuse, vec3 material_specular, float material_shininess) {

    //ambient
    vec3 ambient = lightColor * material_ambient;

    //diffuse
    vec3 norm = normalize(normalOutput);
    vec3 lightDir = normalize(lightPos - posOutput);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material_diffuse);

    //specular
    vec3 viewDir = normalize(viewPos - posOutput);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    vec3 specular = lightColor * (spec * material_specular);

    vec3 result = ambient + diffuse + specular;

    return result;
}

void main()
{
  vec3 color;

  vec3 viewDir = normalize(viewPos - posOutput);
  float nDotv = dot(normalOutput, viewDir);
  float edge = max(nDotv, 0.0);

  if (edge < 0.01) {
    color = vec3(0.0,0.0,0.0);
  } else {
    color = computePhong(lightColor, lightPos, posOutput, normalOutput,
    material_ambient, material_diffuse, material_specular, material_shininess);
  }

  fragColor = vec4(color, 1.0);
}
