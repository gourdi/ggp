	#version 400

uniform vec4 LightPosition;
uniform vec4 LightColor;

in vec4 fragColor;
in vec4 fragPosition;
in vec4 fragNormal;

//out vec4 finalColor;
layout(location = 0) out vec4 finalColor;

void main(void)
{
  vec3 lightDir = normalize(vec3(LightPosition) - vec3(fragPosition));
  float diffuse = clamp(dot(vec3(fragNormal), lightDir), 0, 1);
  
  finalColor = vec4(vec3(fragColor * LightColor) * diffuse, 1);
}