var radius;
var velocity;
var radius_attractor;
var angle;
var attractor_color;

// v in [0, 1], result in [0, 1]
function ease_inout(v)
{
  var sq = v * v;
  return -2 * v * sq + 3 * sq; // -2 * t^3 + 3 * t^2
}

function paint_glow(imageData, center_x, center_y, inner_radius, outter_radius, rgb)
{
  var x_inf = Math.round(center_x - outter_radius);
  if (x_inf > canvas.width - 1)
    return;

  var x_sup = Math.round(center_x + outter_radius);
  if (x_sup < 0)
    return;
  
  var y_inf = Math.round(center_y - outter_radius);
  if (y_inf > canvas.height - 1)
    return;

  var y_sup = Math.round(center_y + outter_radius);
  if (y_sup < 0)
    return;
  
  x_inf = Math.max(x_inf, 0);
  x_sup = Math.min(x_sup, canvas.width - 1);
  y_inf = Math.max(y_inf, 0);
  y_sup = Math.min(y_sup, canvas.height - 1);
  
  var data = imageData.data;
  
  var x, y;
  for (y = y_inf; y <= y_sup; ++y)
  {
    var dy = y + 0.5 - center_y;
    
    for (x = x_inf; x <= x_sup; ++x)
    {
      var dx = x + 0.5 - center_x;
      var dist = Math.sqrt(dx * dx + dy * dy);
      
      if (dist > outter_radius)
        continue;
      
      var paint_alpha = 1;
      if (dist > inner_radius)
      {   
        paint_alpha = 1 - (dist - inner_radius) / (outter_radius - inner_radius);
        paint_alpha = ease_inout(paint_alpha);
      }

      data[4 * (y * imageData.width + x) + 0] = Math.min(data[4 * (y * imageData.width + x) + 0] + paint_alpha * rgb.r, 255);
      data[4 * (y * imageData.width + x) + 1] = Math.min(data[4 * (y * imageData.width + x) + 1] + paint_alpha * rgb.g, 255);
      data[4 * (y * imageData.width + x) + 2] = Math.min(data[4 * (y * imageData.width + x) + 2] + paint_alpha * rgb.b, 255);
    }
  }
}

function init()
{
  radius = random(0, 0.5);
  velocity = 0;
  radius_attractor = random(0.1, 0.5);
  angle = random(0, 2 * Math.PI);
  attractor_color = hsv2rgb(random(0, 1), 1, 0.2);
  
  fill_solid({ r:0, g:0, b:0 });
}

var paint_radius = 0.005 * min_size();
var angle_inc = 0.05;

function render_frame(frame_index)
{
  // Update data.
  if ((frame_index % 100) == 0)
  {
    radius_attractor = random(0.1, 0.5);
    attractor_color = hsv2rgb(random(0, 1), 1, 0.2);
  }
  
  var force = (radius_attractor > radius ? 0.001 : -0.001);
  var radius_prv = radius;
  radius += velocity + force;
  velocity = radius - radius_prv;

  var angle_prv = angle;
  angle += angle_inc;
  
  // Paint.
  var imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);

  var step;
  var steps_count = 6;
  for (step = 0; step < steps_count; ++step)
  {
    var angle_cur = angle_prv + step * angle_inc / steps_count;
    
    // Paint attractor.
    {
      var x = canvas.width  / 2 + radius_attractor * min_size() * Math.cos(angle_cur) / 2;
      var y = canvas.height / 2 + radius_attractor * min_size() * Math.sin(angle_cur) / 2;

      paint_glow(imageData, x, y, paint_radius, 2 * paint_radius, {r:32, g:32, b:32});
      paint_glow(imageData, canvas.width - x, y, paint_radius, 2 * paint_radius, {r:32, g:32, b:32});
    }

    // Paint attracted.
    {
      var radius_cur = radius_prv + step * (radius - radius_prv) / steps_count;
    
      var x = canvas.width  / 2 + radius_cur * min_size() * Math.cos(angle_cur) / 2;
      var y = canvas.height / 2 + radius_cur * min_size() * Math.sin(angle_cur) / 2;

      paint_glow(imageData, x, y, paint_radius, 2 * paint_radius, attractor_color);
      paint_glow(imageData, canvas.width - x, y, paint_radius, 2 * paint_radius, attractor_color);
    }
  }
  
  ctx.putImageData(imageData, 0, 0);
  
  return frame_index < 300;
}
