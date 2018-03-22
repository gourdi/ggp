param([string]$InputFile, [string]$OutputBasename, [string]$Logo, [string]$Play)

Write-Host " Generating TGA images ... " -ForegroundColor DarkBlue -BackgroundColor White
ffmpeg -i $InputFile _%05d.tga

Write-Host " Creating thunmbnail ... " -ForegroundColor DarkBlue -BackgroundColor White
$OutputJpg = $OutputBasename + ".jpg"
gm.exe convert -resize 1066x600! -crop 600x600+223+0 _00050.tga -page +265+260 $Play -flatten $OutputJpg

Write-Host " Adding logo ... " -ForegroundColor DarkBlue -BackgroundColor White
Get-ChildItem _*.tga | ForEach-Object { `
  Write-Host -NoNewline "."; `
	gm.exe convert -resize 640x360! $_ -page +10+10 $Logo -flatten tmp.tga; `
  Remove-Item $_; `
  Move-Item tmp.tga $_; `
}
Write-Host ""

Write-Host " Creating mp4 file ... " -ForegroundColor DarkBlue -BackgroundColor White
$OutputMp4 = $OutputBasename + ".mp4"
ffmpeg -i "_%05d.tga" -r 25 -c:v libx264 -pix_fmt yuv420p -profile:v baseline -crf 20 $OutputMp4

Remove-Item _*.tga
