<?php

function social_footer($url)
{
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



?>
