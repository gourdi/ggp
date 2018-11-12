<?php

$artist = $_GET["artist"];
$video = $_GET["video"];

echo "<!DOCTYPE html>\n<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<style>\n";
echo "body { margin: 0; }\n";
echo "#myVideo { position:absolute; top:0; left:0; width:100%; height:100%; }\n";
echo "</style>\n</head>\n<body style=\"background-color:black;\">\n";

echo "<video autoplay muted loop id=\"myVideo\">\n";
echo "  <source src=\"../artists/$artist/video/$video.mp4\" type=\"video/mp4\">\n";
echo "  Your browser does not support HTML5 video.\n";
echo "</video>\n";

echo "</body>\n";
echo "</html>\n";
