<?php

$artist = $_GET["artist"];
$video = $_GET["video"];

include_once("helpers.php");

html_header($artist, "artists/$artist/$video.jpg");

echo "<body>\n";
echo "<video controls=\"controls\">\n";
echo "<source src=\"artists/$artist/$video.mp4\" type=\"video/mp4\" />\n";
echo "</video>\n";

// Footer.
$url = "http://www.gourdi.net/video.php?artist=$artist&video=$video";
social_footer($url);

echo "</body></html>\n";

