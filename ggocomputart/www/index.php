<?php

$artists = array_diff(scandir("artists"), array(".", ".."));

echo "<!DOCTYPE html>
<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"gourdi.css\" />
<title>Gourdi - Digital art</title>
<meta name=\"viewport\" content=\"width=700, user-scalable=false;\">
<meta name=\"keywords\" content=\"Digital art, computer art, automatic art, algorithmic art, procedural images, procedural videos\">
<meta name=\"descripion\" content=\"Procedurally generated images and videos\">
</head>
<body>
<table class=\"common\"> 
  <tr>
    <td class=\"header-left\">
        <a href=\"http://www.gourdi.net\" style=\"text-decoration: none\">gourdi.net</a>
    </td>
    <td class=\"header-right\">
        <a href=\"mailto:g.gourdin@gmail.com\" style=\"text-decoration: none\">@</a>
    </td>
  </tr>
";

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

echo "</table><br></body></html>";

?>
