ffmpeg -i %08d.bmp -r 25 -c:v libx264 -pix_fmt yuv420p -profile:v baseline -crf 20 test.mp4
del *.bmp
