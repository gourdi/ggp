<?php

$artist = $_GET["artist"];
$video = $_GET["video"];

echo "
<!DOCTYPE html>
<html>
<head>
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">

<style>
body { margin:0; background-color:black; }
video { position:absolute; top:0; left:0; width:100%; height:100%; }
</style>

</head>
<body>

<video autoplay muted loop>
  <source src=\"../artists/$artist/video/$video.mp4\" type=\"video/mp4\">
  Your browser does not support HTML5 video.
</video>

</body>
</html>";

?>
