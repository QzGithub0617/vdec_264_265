#########################################################################
# File Name:    start.sh
# Author:       Mr.Qin
# Created Time: 2022年06月21日 星期二 05时08分17秒
#########################################################################
#!/bin/bash

rm -rf *.yuv CMakeCache.txt ffmpeg_demo cmake_install.cmake CMakeFiles
cmake .
make
./ffmpeg_demo

