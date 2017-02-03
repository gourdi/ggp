#!/bin/bash

ffmpeg -i %08d.bmp -r 25 -qscale 1 -vcodec mpeg4 -y test.mp4
rm *.bmp