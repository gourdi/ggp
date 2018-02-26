<?php

$email = $_POST['email'];
$subject = $_POST['subject'];
$message = $_POST['message'];
$artist = $_GET['artist'];
if (empty($artist)) {
	$artist = $_POST['artist'];
}

$body="<body>";
if (!empty($subject) && !empty($message)) {
	mail("g.gourdin@laposte.net", addslashes($subject), $email."\n\n".addslashes($message));
	mail("postmaster@gourdi.net", addslashes($subject), addslashes($message));
	$body="<body onLoad=\"redirect()\">";
}

echo "<!DOCTYPE html>\n";
echo "<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"gourdi.css\" /><title>Gourdi - Contact</title>\n\n";

echo "<script>\n";
echo "function validateForm() {\n";
echo "var subject=document.forms[\"contact_form\"][\"subject\"].value;\n";
echo "var message=document.forms[\"contact_form\"][\"message\"].value;\n";
echo "if (subject==null || subject==\"\") { alert(\"Please fill subject.\"); return false; }\n";
echo "if (message==null || message==\"\") { alert(\"Please fill message.\"); return false; }\n";
echo "return true; }\n";
echo "function redirect() { alert('Message sent.'); window.location=\"artist.php?artist=$artist\" }\n";
echo "</script>\n\n";

echo "</head>$body";

echo "<table style=\"margin-left:auto; margin-right:auto;\"><tr><td>\n";
echo "<div class=\"title\">Contact</div></td></tr><tr><td>\n";

echo "<div style=\"background-color:#dddddd; border:1px solid #aaaaaa; padding:10px; font-size:75%\">\n";
echo "<form name=\"contact_form\" action=\"contact.php\" method=\"post\" onsubmit=\"return validateForm()\">\n";
echo "Your e-mail (optionnal, required if you want an answer) :<br/>\n";
echo "<input type=\"text\" name=\"email\" size=\"50\" /><br/><br/>\n";
echo "Subject :<br/>\n";
echo "<input type=\"text\" name=\"subject\" size=\"50\"/><br/><br/>\n";
echo "Message :<br/>\n";
echo "<textarea name=\"message\" rows=\"20\" cols=\"70\" ></textarea><br/><br/>\n";
echo "<div style=\"text-align:center;\"><input type=\"submit\" value=\"Submit\"></div>\n"; 
echo "<input type=\"hidden\" name=\"artist\" value=\"$artist\">\n";
echo "</form></div>\n";

echo "</td></tr></table>";

echo "</body></html>";

?>
