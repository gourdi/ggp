<?php

$artist = $_GET["artist"];

include_once("helpers.php");

html_header($artist, "artists/$artist/00000001.jpg");

$cells = array();

// Process images.
for ($i = 1; true; ++$i) {
    
    $filename = sprintf("artists/$artist/%08d.jpg", $i);
    $filename_fullscreen = sprintf("artists/$artist/%08d-fullscreen.jpg", $i);

    if (!file_exists($filename)) {
        break;
    }
    if (!file_exists($filename_fullscreen)) {
        break;
    }

    $image = sprintf("%08d", $i);

    array_push($cells, "<a href=\"image.php?artist=$artist&image=$image\"><img src=\"$filename\" class=\"common\"/></a>");
}

// Process videos.
$videos = array();
for ($i = 1; true; ++$i) {
    
    $filename_mp4 = sprintf("artists/$artist/video%02d.mp4", $i);
    $filename_jpg = sprintf("artists/$artist/video%02d.jpg", $i);
    
    if (!file_exists($filename_mp4)) {
        break;
    }
    if (!file_exists($filename_jpg)) {
        break;
    }

    $video = sprintf("video%02d", $i);

    array_push($videos, "<a href=\"video.php?artist=$artist&video=$video\"><img src=\"$filename_jpg\" class=\"common\"/></a>");
}
array_splice($cells, 1, 0, $videos);

// Make sure we have an even number of cells.
if ((count($cells) % 2) == 1) {
    array_pop($cells);
}

echo "<body>\n";

include_once("analyticstracking.php");

echo "<table class=\"common\">\n";

$i = 0;
foreach ($cells as $cell) {
    
    if (($i % 2) == 0) {
        echo "<tr>\n";
    }

    echo "<td class=\"common\">$cell</td>\n";
	 
    if (($i % 2) == 1) {
        echo "</tr>\n";
    }

    $i += 1;
}

echo "</table>\n";

// Footer.
social_footer("http://www.gourdi.net/artist.php?artist=$artist");

echo "</body></html>";

?>
