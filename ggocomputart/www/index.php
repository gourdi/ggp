<?php

$artists = array_diff(scandir("artists"), array(".", ".."));

echo "<!DOCTYPE html>\n";
echo "<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"gourdi.css\" />\n";
echo "<title>Gourdi - Digital art</title>\n";
echo "<meta name=\"viewport\" content=\"width=700, user-scalable=false;\">";
echo "<meta name=\"keywords\" content=\"Digital art, computer art, automatic art, algorithmic art, procedural images, procedural videos\">\n";
echo "<meta name=\"descripion\" content=\"Procedurally generated images and videos\">\n";
echo "</head><body>\n";
echo "<table class=\"common\">\n";

$i = 0;
foreach ($artists as $artist) {
    if (($i % 2) == 0) {
        echo "<tr>\n";
    }
    echo "<td class=\"common\"><a href=\"artist.php?artist=$artist\">\n";
    if (file_exists("artists/$artist/video/00000001-video-thumbnail.jpg"))
        echo "<img src=\"artists/$artist/video/00000001-video-thumbnail.jpg\" class=\"common\"/></a>\n";
    else
        echo "<img src=\"artists/$artist/image/00000001.jpg\" class=\"common\"/></a>\n";

	echo "<br><p class=\"caption\">$artist</p></td>\n";
    if (($i % 2) == 1) {
        echo "</tr>\n";
    }
    $i += 1;
}
if (($i % 2) == 1) {
    echo "<td></td></tr>";
}

echo "</table><br>";

// Footer.
echo "<table class=\"common\"><tr>\n";
echo "<td><a href=\"faq.html\"><img src=\"res/faq_logo.png\" class=\"footer\"></a><br><p class=\"footer\">FAQ</p></td>\n";
echo "<td><a href=\"https://twitter.com/GourdiNet\"><img src=\"res/twitter_logo.png\" class=\"footer\"></a><br><p class=\"footer\">Twitter</p></td>\n";
echo "<td><a href=\"mailto:g.gourdin@gmail.com\"><img src=\"res/email_logo.png\" class=\"footer\"></a><br><p class=\"footer\">Contact</p></td>\n";
echo "</tr></table>\n";

echo "</body></html>";

?>
