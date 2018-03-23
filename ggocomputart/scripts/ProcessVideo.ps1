param([string]$InputFile, [int]$OutputIndex, [string]$Logo, [int]$PreviewFrame)

$OutputBasename = $OutputIndex.ToString("00000000")

Write-Host " Generating TGA images ... " -ForegroundColor DarkBlue -BackgroundColor White
ffmpeg -i $InputFile _%08d.tga

$PreviewIn = "_" + $PreviewFrame.ToString("00000000") + ".tga"
if ($PreviewFrame -eq 0) { $Files = Get-ChildItem _*.tga; $PreviewIn = $Files[-1] }

Write-Host " Creating thumbnail image ... " -ForegroundColor DarkBlue -BackgroundColor White
$PreviewOut = $OutputBasename + "-video-thumbnail.jpg"
gm.exe convert -resize 1066x600! -crop 600x600+223+0 $PreviewIn $PreviewOut

Write-Host " Adding logo ... " -ForegroundColor DarkBlue -BackgroundColor White
Get-ChildItem _*.tga | ForEach-Object { `
  Write-Host -NoNewline "."; `
	gm.exe convert -resize 640x360! $_ -page +10+10 $Logo -flatten tmp.tga; `
  Remove-Item $_; `
  Move-Item tmp.tga $_; `
}
Write-Host ""

Write-Host " Creating video preview image ... " -ForegroundColor DarkBlue -BackgroundColor White
$PreviewOut = $OutputBasename + "-video-preview.jpg"
gm.exe convert $PreviewIn -quality 90 $PreviewOut

Write-Host " Creating mp4 file ... " -ForegroundColor DarkBlue -BackgroundColor White
$OutputMp4 = $OutputBasename + ".mp4"
ffmpeg -i "_%08d.tga" -r 25 -c:v libx264 -pix_fmt yuv420p -profile:v baseline -crf 20 $OutputMp4

Remove-Item _*.tga
