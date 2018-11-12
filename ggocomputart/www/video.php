<?php

$artist = $_GET["artist"];
$video = $_GET["video"];

echo "<!DOCTYPE html>\n<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<style>\n";
echo "body { margin: 0; }\n";
echo "#myVideo { position: fixed; right: 0; bottom: 0; min-width: 100%; min-height: 100%; }\n";
echo "</style>\n</head>\n<body>\n";

echo "<video autoplay muted id=\"myVideo\">\n";
echo "  <source src=\"../artists/$artist/video/$video.mp4\" type=\"video/mp4\">\n";
echo "  Your browser does not support HTML5 video.\n";
echo "</video>\n";

echo "</body>\n";
echo "</html>\n";
