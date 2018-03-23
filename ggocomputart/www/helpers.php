<?php

function social_footer($url) {

    $encoded_url = urlencode($url);

	echo "<table class=\"footer\"><tr>\n";

	echo "<td><a href=\"https://twitter.com/intent/tweet?text=$encoded_url\">\n";
	echo "<img src=\"res/twitter_logo.png\" class=\"footer\"></a><br>\n";
	echo "<p class=\"footer\">Share on<br>Twitter</p></td>\n";

	echo "<td><a href=\"https://www.facebook.com/sharer/sharer.php?u=$encoded_url\">\n";
	echo "<img src=\"res/facebook_logo.png\" class=\"footer\"></a><br>\n";
	echo "<p class=\"footer\">Share on<br>Facebook</p></td>\n";

	echo "<td><a href=\"https://plus.google.com/share?url=$encoded_url\">\n";
	echo "<img src=\"res/googleplus_logo.png\" class=\"footer\"></a><br>\n";
	echo "<p class=\"footer\">Share on<br>Google+</p></td>\n";

	echo "</tr></table>\n";
}

function html_header($title, $meta_image) {

    echo "<!DOCTYPE html>\n";
    echo "<html itemscope itemtype=\"http://schema.org/Article\">\n";
    echo "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"gourdi.css\" />\n";
    echo "<title>$title</title>\n";
  //  echo "<meta name=\"viewport\" content=\"width=700, user-scalable=false;\" />\n";
  //  echo "<meta itemprop=\"name\" content=\"$title\" />\n";
  //  echo "<meta itemprop=\"description\" content=\"From http://www.gourdi.net\" />\n";
 //   echo "<meta itemprop=\"image\" content=\"$meta_image\" />\n";
    echo "</head>\n";
}

?>
