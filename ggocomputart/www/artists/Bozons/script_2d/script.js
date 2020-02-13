function bozon(cur_pos_x, cur_pos_y, angle, dangle, speed, radius, color, counter)
{
  this.cur_pos_x = cur_pos_x;
  this.cur_pos_y = cur_pos_y;
  this.angle = angle;
  this.dangle = dangle;
  this.speed = speed;
  this.radius = radius;
  this.color = color;
  this.counter = counter;
}

var bozons = [];

function init()
{
  var hue = rand(0, 1);
  
  // Background.
  fill_4_colors(
    hsv2rgb(hue, rand(0, 1), rand(0, 1)),
    hsv2rgb(hue, rand(0, 1), rand(0, 1)),
    hsv2rgb(hue, rand(0, 1), rand(0, 1)),
    hsv2rgb(hue, rand(0, 1), rand(0, 1))); 

  // Create bozons.
  bozons = [];
  for (; bozons.length < 25;)
  {
    var cur_pos_x = canvas.clientWidth / 2;
    var cur_pos_y = canvas.clientHeight / 2;
    var angle = rand(0, 2 * Math.PI);
    var dangle = rand(-0.025 * Math.PI, 0.025 * Math.PI);
    var speed = rand(0.01, 0.02) * min_size();
    var radius = 0.0025 * min_size();
    var rgb = hsv2rgb(hue, rand(0, 1), rand(0, 1), 100);
    var color = 'rgba(' + rgb.r + ',' + rgb.g + ',' + rgb.b + ',40)'; 
    var counter = rand(200, 300);

    bozons.push(new bozon(cur_pos_x, cur_pos_y, angle, dangle, speed, radius, color, counter));
  }
}

function render_frame(frame_index)
{
  var i;
  for (i = bozons.length - 1; i >= 0; i--)
  {
    var cur_bozon = bozons[i];
    
    // Check if the bozon is still alive.
    --cur_bozon.counter;
    if (cur_bozon.counter < 0)
    {
      bozons.splice(i, 1);
      continue;
    }
  
    // Update the current bozon.
    var prv_pos_x = cur_bozon.cur_pos_x;
    var prv_pos_y = cur_bozon.cur_pos_y;

    cur_bozon.angle += cur_bozon.dangle;
    cur_bozon.dangle *= 1.01;
    cur_bozon.speed *= 0.99;
    cur_bozon.radius *= 0.995;
    cur_bozon.cur_pos_x += cur_bozon.speed * Math.cos(cur_bozon.angle);
    cur_bozon.cur_pos_y += cur_bozon.speed * Math.sin(cur_bozon.angle);
    
    // Split bozon?
    if (frame_index > 30 && rand(0, 1) < 0.01)
    {
      var angle_offset = rand(0, Math.PI / 4);
      
      var angle = cur_bozon.angle + angle_offset;
      var dangle = -cur_bozon.dangle; // Change direction too.
      
      cur_bozon.angle -= angle_offset;

      bozons.push(new bozon(cur_bozon.cur_pos_x, cur_bozon.cur_pos_y, angle, dangle, 
        cur_bozon.speed, cur_bozon.radius, cur_bozon.color, cur_bozon.counter));
    }

    // Paint.
    var dx = cur_bozon.cur_pos_x - prv_pos_x;
    var dy = cur_bozon.cur_pos_y - prv_pos_y;
    var step = 8;
    for (; step > 0; --step)
    {
      ctx.beginPath();
      ctx.fillStyle = cur_bozon.color;
      ctx.arc(prv_pos_x + step * dx / 8, prv_pos_y + step * dy / 8, cur_bozon.radius, 0, 2 * Math.PI);
      ctx.fill();
    }
  }
  
  return bozons.length > 0;
}


