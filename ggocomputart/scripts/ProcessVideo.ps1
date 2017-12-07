param([string]$InputFile, [string]$OutputBasename, [string]$Logo, [string]$Play)

ffmpeg -i $InputFile _%05d.tga

$OutputJpg = $OutputBasename + ".jpg"
convert _00050.tga -resize 1066x600! -crop 600x600+223+0 +repage -page +265+260 $Play -flatten $OutputJpg

Get-ChildItem _*.tga | ForEach-Object { `
	convert.exe $_ -resize 640x360! -page +10+10 $Logo -flatten -flip tmp.tga; `
  Remove-Item $_; `
  Move-Item tmp.tga $_; `
}

$OutputMp4 = $OutputBasename + ".mp4"
$OutputOgg = $OutputBasename + ".ogg"
ffmpeg -i "_%05d.tga" -r 25 -q:v 6 -c:v libx264 -y $OutputMp4
ffmpeg -i "_%05d.tga" -r 25 -q:v 6 -c:v libtheora -y $OutputOgg

Remove-Item _*.tga
