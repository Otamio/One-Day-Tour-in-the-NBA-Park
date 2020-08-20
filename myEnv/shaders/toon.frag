#version 330 core

in vec3 posOutput;
in vec3 normalOutput;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;
uniform bool normal_coloring;
uniform vec3 viewPos;
uniform bool islight;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec2 lightBias;
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

  color = computePhong(lightColor, lightPos, posOutput, normalOutput,
  material_ambient, material_diffuse, material_specular, material_shininess);

  vec3 unitLightVector = normalize(lightPos - posOutput);
  float nDotl = dot(normalOutput, unitLightVector);
  float brightness = max(nDotl, 0.0);

  if (brightness > 0.95)
		color = vec3(1.0,0.5,0.5) * color;
	else if (brightness > 0.5)
		color = vec3(0.6,0.3,0.3) * color;
	else if (brightness > 0.25)
		color = vec3(0.4,0.2,0.2) * color;
	else
		color = vec3(0.2,0.1,0.1) * color;

  fragColor = vec4(color, 1.0);
}
