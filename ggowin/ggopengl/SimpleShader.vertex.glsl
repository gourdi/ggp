#version 400

layout(location=0) in vec4 vertPosition;
layout(location=1) in vec4 vertColor;
layout(location=2) in vec4 vertNormal;
out vec4 fragPosition;
out vec4 fragColor;
out vec4 fragNormal;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
  fragPosition = vertPosition;
  fragColor = vertColor;
  fragNormal = vertNormal;

  gl_Position = (ProjectionMatrix * ViewMatrix) * vertPosition;
}