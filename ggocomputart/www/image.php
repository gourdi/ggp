<?php

$artist = $_GET["artist"];
$image = $_GET["image"];

include_once("helpers.php");

html_header($artist, "artists/$artist/$image.jpg");

echo "<body>\n";
echo "<img class=\"fullscreen\" src=\"artists/$artist/$image-fullscreen.jpg\"><br>\n";

// Footer.
$url = "http://www.gourdi.net/image.php?artist=$artist&image=$image";
social_footer($url);

echo "</body></html>\n";
