function mapLength(length, width, height)
{
  if (width > height)
  {
    return length * width;
  }
  else
  {
    return length * height;
  }
}

function mapPoint(p, width, height)
{
  if (width > height)
  {
    var x = width * p[0];
    var y = width * p[1];
    return [x, y - (width - height) / 2];
  }
  else
  {
    var x = height * p[0];
    var y = height * p[1];
    return [x - (height - width) / 2, y];
  }
}

var circles = [];

function init()
{
  circles = [];
  
  for (var i = 0; i < 32; ++i)
  {
    var circle = {
      center_start: [ rand(-0.25, 1.25), rand(-0.25, 1.25) ],
      center_end: [ rand(-0.25, 1.25), rand(-0.25, 1.25) ],
      r_angle_start: rand(0, Math.PI),
      g_angle_start: rand(0, Math.PI),
      b_angle_start: rand(0, Math.PI),
      r_angle_end: rand(0, Math.PI),
      g_angle_end: rand(0, Math.PI),
      b_angle_end: rand(0, Math.PI),
      radius: rand(0.1, 0.15),
      inner_size: rand(0.01, 0.02),
      outter_size: rand(0.002, 0.04),
      opacity: rand(0.25, 0.75)
    };
    circles.push(circle);
  }
}

function get_frament_shader()
{
  var fs =  
  `
  uniform lowp float min_size;
  uniform lowp float params[10*32*3];

  lowp float ease_inout(lowp float t)
  {
    lowp float sq = t * t;
    return -2.0 * t * sq + 3.0 * sq; // -2 * t^3 + 3 * t^2
  }

  void main()
  {
    lowp vec3 c = vec3(0, 0, 0);

    for (int i = 0; i < 32*3; ++i)
    {
      lowp vec2 center = vec2(params[10 * i + 0], params[10 * i + 1]);
      lowp vec3 color = vec3(params[10 * i + 2], params[10 * i + 3], params[10 * i + 4]);

      lowp float radius0 = params[10 * i + 5];
      lowp float radius1 = params[10 * i + 6];
      lowp float radius2 = params[10 * i + 7];
      lowp float radius3 = params[10 * i + 8];
      lowp float outter_size = params[10 * i + 9];

      lowp float d = distance(center, vec2(gl_FragCoord.x, gl_FragCoord.y));
      lowp float r = 0.0;

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
          r = 1.0;
        }

        c += r * color;
      }

    }

    gl_FragColor = vec4(c, 1);
  }
  `;
  
  return fs;
}

function set_uniforms(gl, program, progress)
{
  // The smallest dimension of the canvas.
  var min_size_uniform = gl.getUniformLocation(program, 'min_size');
  gl.uniform1f(min_size_uniform, Math.min(canvas.width, canvas.height));

  // All the circles as an aray of floats.
  var paramsUniform = gl.getUniformLocation(program, 'params');
  var params = new Float32Array(10*32*3);
  
  var offset = mapLength(0.01, canvas.width, canvas.height);
  for (let i = 0; i < 32; i++)
  {
    circle = circles[i];
    
    center_x = linerp(circle.center_start[0], circle.center_end[0], progress);
    center_y = linerp(circle.center_start[1], circle.center_end[1], progress);
    center = mapPoint([center_x, center_y], canvas.width, canvas.height);
    
    r_angle = linerp(circle.r_angle_start, circle.r_angle_end, progress);
    g_angle = linerp(circle.g_angle_start, circle.g_angle_end, progress);
    b_angle = linerp(circle.b_angle_start, circle.b_angle_end, progress);
    
    center1_x = center[0] + offset * Math.cos(r_angle);
    center1_y = center[1] + offset * Math.sin(r_angle);
    center2_x = center[0] + offset * Math.cos(g_angle);
    center2_y = center[1] + offset * Math.sin(g_angle);
    center3_x = center[0] + offset * Math.cos(b_angle);
    center3_y = center[1] + offset * Math.sin(b_angle);
    
    params[30*i+0] = center1_x;
    params[30*i+1] = center1_y;
    params[30*i+2] = circle.opacity;
    params[30*i+3] = 0;
    params[30*i+4] = 0;
    params[30*i+5] = mapLength(circle.radius - circle.outter_size - circle.inner_size, canvas.width, canvas.height);
    params[30*i+6] = mapLength(circle.radius - circle.inner_size, canvas.width, canvas.height);
    params[30*i+7] = mapLength(circle.radius + circle.inner_size, canvas.width, canvas.height);
    params[30*i+8] = mapLength(circle.radius + circle.outter_size + circle.inner_size, canvas.width, canvas.height);
    params[30*i+9] = mapLength(circle.outter_size, canvas.width, canvas.height);

    params[30*i+10] = center2_x;
    params[30*i+11] = center2_y;
    params[30*i+12] = 0;
    params[30*i+13] = circle.opacity;
    params[30*i+14] = 0;
    params[30*i+15] = mapLength(circle.radius - circle.outter_size - circle.inner_size, canvas.width, canvas.height);
    params[30*i+16] = mapLength(circle.radius - circle.inner_size, canvas.width, canvas.height);
    params[30*i+17] = mapLength(circle.radius + circle.inner_size, canvas.width, canvas.height);
    params[30*i+18] = mapLength(circle.radius + circle.outter_size + circle.inner_size, canvas.width, canvas.height);
    params[30*i+19] = mapLength(circle.outter_size, canvas.width, canvas.height);
    
    params[30*i+20] = center3_x;
    params[30*i+21] = center3_y;
    params[30*i+22] = 0;
    params[30*i+23] = 0;
    params[30*i+24] = circle.opacity;
    params[30*i+25] = mapLength(circle.radius - circle.outter_size - circle.inner_size, canvas.width, canvas.height);
    params[30*i+26] = mapLength(circle.radius - circle.inner_size, canvas.width, canvas.height);
    params[30*i+27] = mapLength(circle.radius + circle.inner_size, canvas.width, canvas.height);
    params[30*i+28] = mapLength(circle.radius + circle.outter_size + circle.inner_size, canvas.width, canvas.height);
    params[30*i+29] = mapLength(circle.outter_size, canvas.width, canvas.height);
  }

  gl.uniform1fv(paramsUniform, params);
}
