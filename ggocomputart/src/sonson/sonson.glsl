R"(#version 330 core

in vec4 gl_FragCoord;
uniform ivec2 resolution;
uniform float params[10*32*3];
out vec4 glFragColor;

float ease_inout(float t)
{
  float sq = t * t;
  return -2 * t * sq + 3 * sq; // -2 * t^3 + 3 * t^2
}

void main()
{
  vec3 c = vec3(0, 0, 0);
  float min_size = min(resolution.x, resolution.y);

  for (int i = 0; i < 32*3; ++i)
  {
    vec2 center = vec2(params[10 * i + 0], params[10 * i + 1]);
    vec3 color = vec3(params[10 * i + 2], params[10 * i + 3], params[10 * i + 4]);

    float radius0 = params[10 * i + 5];
    float radius1 = params[10 * i + 6];
    float radius2 = params[10 * i + 7];
    float radius3 = params[10 * i + 8];
    float outter_size = params[10 * i + 9];

    float d = distance(center, vec2(gl_FragCoord.x, gl_FragCoord.y));
    float r = 0.f;

    if (d > radius0 && d < radius3)
    {
      if (d < radius1)
      {
        r = ease_inout((d - radius0) / outter_size);
      }
      else if (d > radius2)
      {
        r = ease_inout((radius3 - d) / outter_size);
      }
      else
      {
        r = 1.f;
      }

      c += r * color;
    }

  }

  gl_FragColor = vec4(c, 1);
})"