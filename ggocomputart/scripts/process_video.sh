#!/bin/bash
set -x

INPUT_FILE=$1
OUTPUT_FILE=$2
LOGO=$3
PLAY=$4

ffmpeg -i $INPUT_FILE _%05d.tga

convert _00050.tga -resize 1066x600\! -crop 600x600+223+0 +repage -page +265+260 $PLAY -flatten $OUTPUT_FILE.jpg

for b in _*.tga; do
	convert $b -resize 640x360\! -page +10+10 $LOGO -flatten -flip tmp.tga
    rm $b
    mv tmp.tga $b
done;

ffmpeg -i "_%05d.tga" -r 25 -q:v 6 -codec:v mpeg4 -y $OUTPUT_FILE.mp4
ffmpeg -i "_%05d.tga" -r 25 -q:v 1 -codec:v mpeg4 -y _hq.mp4
ffmpeg2theora -v 6 _hq.mp4 -o $OUTPUT_FILE.ogg

rm _*.tga _hq.mp4
