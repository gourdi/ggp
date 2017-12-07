param([string]$InputDir, [string]$OutputDir, [string]$Logo)

Get-ChildItem $InputDir | `
Where-Object { $_.Name.EndsWith("png") -or $_.Name.EndsWith("bmp")} | `
ForEach-Object {
  Write-Host "Processing image $_";

  $FullScreenOutput = $OutputDir + '/' + $_.BaseName + "-fullscreen.jpg";
  convert.exe $_ -resize 1000x1000 -page +10+10 $Logo -flatten $FullScreenOutput;
  
  $PreviewOutput = $OutputDir + '/' + $_.BaseName + ".jpg";
  convert.exe $_ -resize 600x600 $PreviewOutput
}

