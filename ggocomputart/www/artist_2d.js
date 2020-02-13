var canvas = document.getElementById('canvas');
var ctx = canvas.getContext('2d');

canvas.width = window.innerWidth;
canvas.height = window.innerHeight;

// resize the canvas to fill browser window dynamically
window.addEventListener('resize', resizeCanvas, false);
function resizeCanvas()
{
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
  
  init();
}

function hsv2rgb(h, s, v)
 {
  var r, g, b, i, f, p, q, t;
  i = Math.floor(h * 6);
  f = h * 6 - i;
  p = v * (1 - s);
  q = v * (1 - f * s);
  t = v * (1 - (1 - f) * s);
  
  switch (i % 6)
  {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
  }

  return {
    r: Math.round(r * 255),
    g: Math.round(g * 255),
    b: Math.round(b * 255)
  };
}

function fill_solid(color)
{
  ctx.rect(0, 0, canvas.clientWidth, canvas.clientHeight);
  ctx.fillStyle = 'rgb(' + color.r + ',' + color.g + ',' + color.b + ')';
  ctx.fill();
}

function fill_4_colors(color1, color2, color3, color4)
{
  var imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
  var data = imageData.data;
  var x, y;
  
  for (y = 0; y < imageData.height; ++y)
  {
    var color5 = { 
      r: (y * color1.r + (imageData.height - y) * color2.r) / imageData.height,
      g: (y * color1.g + (imageData.height - y) * color2.g) / imageData.height,
      b: (y * color1.b + (imageData.height - y) * color2.b) / imageData.height };
    var color6 = { 
      r: (y * color3.r + (imageData.height - y) * color4.r) / imageData.height,
      g: (y * color3.g + (imageData.height - y) * color4.g) / imageData.height,
      b: (y * color3.b + (imageData.height - y) * color4.b) / imageData.height };
    
    for (x = 0; x < imageData.width; ++x)
    {
      data[4 * (y * imageData.width + x) + 0] = Math.round((x * color5.r + (imageData.width - x) * color6.r) / imageData.width);
      data[4 * (y * imageData.width + x) + 1] = Math.round((x * color5.g + (imageData.width - x) * color6.g) / imageData.width);
      data[4 * (y * imageData.width + x) + 2] = Math.round((x * color5.b + (imageData.width - x) * color6.b) / imageData.width);
      data[4 * (y * imageData.width + x) + 3] = 255;
    }
  }

  ctx.putImageData(imageData, 0, 0);
}

var frame_index = 0;

function main_loop()
{
  if (!render_frame(frame_index))
  {
    frame_index = 0;
    init();
  }

  ++frame_index;
}

init();
intervalID = setInterval(main_loop, 25);

