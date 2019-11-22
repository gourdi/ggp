R"(#version 330 core

in vec4 gl_FragCoord;
uniform ivec2 resolution;
uniform float altitude_factor;
uniform vec3 color;
uniform float params[250];
out vec4 glFragColor;

float map(float v, float inf_from, float sup_from, float inf_to, float sup_to)
{ 
  return inf_to + (v - inf_from ) * (sup_to - inf_to ) / (sup_from - inf_from);
}

float compute_altitude(float x, float y)
{
  float z = 0;
  for (int i = 0; i < 50; ++i)
  {
    float den   = params[5 * i + 0];
    float mult  = params[5 * i + 1];
    float power = params[5 * i + 2];
    vec2 pos = vec2(params[5 * i + 3], params[5 * i + 4]);

    vec2 delta = pos - vec2(x, y);

    float h = dot(delta, delta);
    z += den / (0.1f + pow(h * mult, power));
    z += den / (0.1f + pow(h * mult, power));
  }

  return z;
}

void main()
{
  float width_f   = resolution.x;
  float height_f  = resolution.y;
  float range_x   = width_f > height_f ? 1.f : width_f / height_f;
  float range_y   = height_f > width_f ? 1.f : height_f / width_f;

  float y1 = map(gl_FragCoord.y - 3 / 8.f, 0.f, height_f, -range_y, range_y);
  float y2 = map(gl_FragCoord.y + 3 / 8.f, 0.f, height_f, -range_y, range_y);

  float x1 = map(gl_FragCoord.x - 3 / 8.f, 0.f, width_f, -range_x, range_x);
  float x2 = map(gl_FragCoord.x + 3 / 8.f, 0.f, width_f, -range_x, range_x);

  float z11 = compute_altitude(x1, y1);
  float z12 = compute_altitude(x1, y2);
  float z21 = compute_altitude(x2, y1);
  float z22 = compute_altitude(x2, y2);
  
  float dx = x2 - x1;
  float dy = y2 - y1;
  vec3 v1 = vec3(dx, dy, z22 - z11);
  vec3 v2 = vec3(dx, -dy, z21 - z12);
  vec3 normal = normalize(cross(v1, v2));

  vec3 pixel_color = color * (altitude_factor + abs(normal.z) / altitude_factor);

  gl_FragColor = vec4(pixel_color, 1);
})"