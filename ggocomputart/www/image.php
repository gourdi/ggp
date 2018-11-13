<?php

$artist = $_GET["artist"];
$image = $_GET["image"];

echo "
<!DOCTYPE html>
<html>
<head>
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">

<style>
body { margin:0; background-color:black; height:100% }
img { position:fixed; top:0; bottom:0; left:0; right:0; max-width:100%; max-height:100%; margin:auto; }
</style>

</head>
<body>

<img src='artists/$artist/image/$image-fullscreen.jpg'>

</body>
</html>";

?>
