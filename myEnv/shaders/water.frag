#version 330

const vec3 waterColour = vec3(0.604, 0.867, 0.851);

out vec4 out_color;

in vec4 pass_clipSpaceReal;
in vec2 textureCoords;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthTexture;
uniform sampler2D dudvMap;
uniform float moveFactor;
uniform float waterParam;

const float waveStrength = 0.002;

vec2 clipSpaceToTexCoords(vec4 clipSpace){
	vec2 ndc = (clipSpace.xy / clipSpace.w);
	vec2 texCoords = ndc / 2.0 + 0.5;
	return texCoords;
}



void main(void) {

  vec2 texCoordsReal = clipSpaceToTexCoords(pass_clipSpaceReal);

  vec2 refractionTexCoords = texCoordsReal;
  vec2 reflectionTexCoords = vec2(texCoordsReal.x, 1.0-texCoordsReal.y);

  vec2 distortion1 = (texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 2.0 - 1.0) * waveStrength;
  vec2 distortion2 = (texture(dudvMap, vec2(-textureCoords.x + moveFactor, textureCoords.y+moveFactor)).rg * 2.0 - 1.0) * waveStrength;
  vec2 totalDistortion = distortion1 + distortion2;

  refractionTexCoords += totalDistortion;
  refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);
  reflectionTexCoords += totalDistortion;
  //reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999);
  //reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

  vec3 refractColor = texture(refractionTexture, refractionTexCoords).rgb;
	vec3 reflectColor = texture(reflectionTexture, reflectionTexCoords).rgb;

  vec3 finalColor = mix(reflectColor, refractColor, 0.5);

	if (waterParam == 0.0) {
  	out_color = vec4(finalColor, 1.0);
	} else if (waterParam == 1.0) {
		out_color = vec4(reflectColor, 1.0);
	} else {
		out_color = vec4(refractColor, 1.0);
	}
}
