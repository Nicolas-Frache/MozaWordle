#!/bin/bash

gource --key --hide bloom,filenames,progress,mouse,dirnames --disable-bloom --font-size 24 --title "Projet Wordle, groupe 32" --no-vsync --seconds-per-day 0.07 --auto-skip-seconds 1 -1920x1060 --background-colour F6EFDC --user-font-size 35 --camera-mode overview --file-extensions --font-colour 000000 --dir-colour 4d4d4d --filename-colour 4d4d4d -o - | ffmpeg -y -r 25 -f image2pipe -vcodec ppm -i - -vcodec libx264 -preset ultrafast -pix_fmt yuv420p -crf 1 -threads 0 -bf 0 input.mp4
mkdir frames
ffmpeg -i input.mp4 -vf scale=1920:-1:flags=lanczos,fps=10 frames/ffout%03d.png
convert -loop 0 frames/ffout*.png output.gif
rm -rf frames
