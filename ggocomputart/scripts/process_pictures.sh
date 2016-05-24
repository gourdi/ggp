#!/bin/bash

INPUT_DIR=$1
OUTPUT_DIR=$2
LOGO=$3

for b in "$INPUT_DIR"/*.{png,bmp,jpg}; do
    if [ -e $b ]; then
        base=`basename $b`
        echo "Processing $base..."
        base="${base%.*}"
        convert $b -resize 1000x1000 -page +10+10 $LOGO -flatten $OUTPUT_DIR/${base}-fullscreen.jpg
    	convert $b -resize 600x600 $OUTPUT_DIR/${base}.jpg
    fi
done;
