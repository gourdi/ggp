param([string]$InputFile, [string]$OutputBasename, [string]$Logo, [string]$Play)

Write-Host " Generating TGA images ... " -ForegroundColor DarkBlue -BackgroundColor White
ffmpeg -i $InputFile _%05d.tga

Write-Host " Creating thunmbnail ... " -ForegroundColor DarkBlue -BackgroundColor White
$OutputJpg = $OutputBasename + ".jpg"
convert _00050.tga -resize 1066x600! -crop 600x600+223+0 +repage -page +265+260 $Play -flatten $OutputJpg

Write-Host " Adding logo ... " -ForegroundColor DarkBlue -BackgroundColor White
Get-ChildItem _*.tga | ForEach-Object { `
	convert.exe $_ -resize 640x360! -page +10+10 $Logo -flatten -flip tmp.tga; `
  Remove-Item $_; `
  Move-Item tmp.tga $_; `
}

Write-Host " Creating mp4 file ... " -ForegroundColor DarkBlue -BackgroundColor White
$OutputMp4 = $OutputBasename + ".mp4"
ffmpeg -i "_%05d.tga" -r 25 -c:v libx264 -q:v 6 -y $OutputMp4

Write-Host " Creating ogg file ... " -ForegroundColor DarkBlue -BackgroundColor White
$OutputOgg = $OutputBasename + ".ogg"
ffmpeg -i "_%05d.tga" -r 25 -c:v libtheora -q:v 6 -y $OutputOgg

Remove-Item _*.tga
