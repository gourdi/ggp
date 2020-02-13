function min_size()
{
  return Math.min(canvas.width, canvas.height);
}

function rand(inf, sup)
{
  return inf + Math.random() * (sup - inf);
}

function linerp(start, end, progress)
{
  return start * (1 - progress) + end * progress;
}

