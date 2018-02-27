ffmpeg -i %%08d.bmp -r 25 -c:v libx264 -crf 12 test.mp4
del *.bmp /q
