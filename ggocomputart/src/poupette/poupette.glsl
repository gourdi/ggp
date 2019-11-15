R"(#version 330 core

in vec4 gl_FragCoord;
uniform ivec2 resolution;
uniform vec3 c1;
uniform vec3 c2;
uniform vec3 c3;
uniform vec3 c4;
out vec4 glFragColor;

vec3 sample(float x, float y)
{
  float weight_x = x / resolution.x;
  float weight_y = y / resolution.y;
  vec3 c5 = weight_x * c1 + (1 - weight_x) * c2;
  vec3 c6 = weight_x * c3 + (1 - weight_x) * c4;
  vec3 c7 = weight_y * c5 + (1 - weight_y) * c6;
  return vec3(mod(c7.r, 1), mod(c7.g, 1), mod(c7.b, 1));
}

void main()
{
  vec3 c = sample(gl_FragCoord.x - 0.375, gl_FragCoord.y - 0.375);
  c += sample(gl_FragCoord.x - 0.375, gl_FragCoord.y - 0.125);
  c += sample(gl_FragCoord.x - 0.375, gl_FragCoord.y + 0.125);
  c += sample(gl_FragCoord.x - 0.375, gl_FragCoord.y + 0.375);

  c += sample(gl_FragCoord.x - 0.125, gl_FragCoord.y - 0.375);
  c += sample(gl_FragCoord.x - 0.125, gl_FragCoord.y - 0.125);
  c += sample(gl_FragCoord.x - 0.125, gl_FragCoord.y + 0.125);
  c += sample(gl_FragCoord.x - 0.125, gl_FragCoord.y + 0.375);

  c += sample(gl_FragCoord.x + 0.125, gl_FragCoord.y - 0.375);
  c += sample(gl_FragCoord.x + 0.125, gl_FragCoord.y - 0.125);
  c += sample(gl_FragCoord.x + 0.125, gl_FragCoord.y + 0.125);
  c += sample(gl_FragCoord.x + 0.125, gl_FragCoord.y + 0.375);

  c += sample(gl_FragCoord.x + 0.375, gl_FragCoord.y - 0.375);
  c += sample(gl_FragCoord.x + 0.375, gl_FragCoord.y - 0.125);
  c += sample(gl_FragCoord.x + 0.375, gl_FragCoord.y + 0.125);
  c += sample(gl_FragCoord.x + 0.375, gl_FragCoord.y + 0.375);

  gl_FragColor = vec4(c / 16, 1);
})"