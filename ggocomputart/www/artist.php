<?php

$artist = $_GET["artist"];

include_once("helpers.php");

//////////////////////////////////////////////////////////////
function html_header($title, $meta_image)
{
  echo "<!DOCTYPE html>\n";
  echo "<html itemscope itemtype=\"http://schema.org/Article\">\n";
  echo "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"gourdi.css\" />\n";
  echo "<title>$title</title>\n";
  echo "</head>\n";
  
  include_once("analyticstracking.php");
}

//////////////////////////////////////////////////////////////
function process_images_videos($artist)
{
  $cells = array();

  // Process videos.
  for ($i = 1; true; ++$i)
  { 
    $filename_mp4 = sprintf("artists/$artist/video/%08d.mp4", $i);
    $filename_thumbnail = sprintf("artists/$artist/video/%08d-video-thumbnail.jpg", $i);
    $filename_preview = sprintf("artists/$artist/video/%08d-video-preview.jpg", $i);
    
    if (!file_exists($filename_mp4) || !file_exists($filename_thumbnail) || !file_exists($filename_preview))
    {
      break;
    } 
    
    $video = sprintf("%08d", $i);
    
    $div = "<div style=\"position:relative; left:0; top:0;\">".
           "<img src=\"$filename_thumbnail\" class=\"common\"/ style=\"position:relative; top:0; left:0;\"/>".
           "<img src=\"res/play.png\" style=\"position:absolute; top:110px; left:125px;\"/>".
           "</div>";

    array_push($cells, "<a href=\"video.php?artist=$artist&video=$video\">$div</a>");
  }

  // Process images.
  for ($i = 1; true; ++$i)
  {
    $filename = sprintf("artists/$artist/image/%08d.jpg", $i);
    $filename_fullscreen = sprintf("artists/$artist/image/%08d-fullscreen.jpg", $i);

    if (!file_exists($filename) || !file_exists($filename_fullscreen))
    {
      break;
    }

    $image = sprintf("%08d", $i);

    array_push($cells, "<a href=\"image.php?artist=$artist&image=$image\"><img src=\"$filename\" class=\"common\"/></a>");
  }

  // Make sure we have an even number of cells.
  if ((count($cells) % 2) == 1)
  {
    array_pop($cells);
  }
  
  // Now we can generate the HTML.
  html_header($artist, "artists/$artist/images/00000001.jpg");

  echo "<body>\n";
  echo "<table class=\"common\">\n";

  $i = 0;
  foreach ($cells as $cell)
  {
    if (($i % 2) == 0)
    {
      echo "<tr>\n";
    }

    echo "<td class=\"common\">$cell</td>\n";
     
    if (($i % 2) == 1)
    {
      echo "</tr>\n";
    }

    $i += 1;
  }

  echo "</table>\n";
  social_footer("http://www.gourdi.net/artist.php?artist=$artist");
  echo "\n</body></html>";
}

//////////////////////////////////////////////////////////////
function process_script($artist)
{
echo "  <!DOCTYPE html>
<html>
";

echo "
<head>
  <style>
    * { margin:0; padding:0; }
    html, body { width:100%; height:100%; }
    canvas { display:block; }
  </style>
</head>

<body> 
<canvas id='canvas'></canvas>
</body>

<script type='text/javascript'>
";

include_once("helpers.js");
include_once("artists/$artist/script/script.js");
echo "

</script>
</html>";
}

//////////////////////////////////////////////////////////////
if (file_exists("artists/$artist/script/script.js"))
{
  process_script($artist);
}
else
{
  process_images_videos($artist);
}

?>
