R"(#version 330 core
in vec4 gl_FragCoord;
uniform ivec2 resolution;
uniform float stripes[128];
uniform float transforms[128];
out vec4 glFragColor;

vec3 sample(float x, float y)
{
  vec2 xy = vec2(x, y) / resolution;
  x = xy.x;
  for (int i = 0; i < 32; ++i)
  {
    vec2 diff = xy - vec2(transforms[4 * i + 0], transforms[4 * i + 1]);
    
    float influence = exp(-dot(diff, diff) / transforms[4 * i + 3]);
      
    x += influence * transforms[4 * i + 2];
  }
  x = mod(x, 1);

  vec3 c;
  for (int i = 0; i < 32; ++i)
  {
    if (stripes[4 * i] >= x)
      break;
    c = vec3(stripes[4 * i + 1], stripes[4 * i + 2], stripes[4 * i + 3]);
  }  

  return c;
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

//  vec3 c = sample(gl_FragCoord.x - 0.25, gl_FragCoord.y - 0.25);
//  c += sample(gl_FragCoord.x - 0.25, gl_FragCoord.y + 0.25);
//  c += sample(gl_FragCoord.x + 0.25, gl_FragCoord.y - 0.25);
//  c += sample(gl_FragCoord.x + 0.25, gl_FragCoord.y + 0.25);
//
//  gl_FragColor = vec4(c / 4, 1);
})"