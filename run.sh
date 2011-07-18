#!/usr/bin/env bash

cd .
cd src
make clean
sleep 1
make
./rayTracer
make clean

cd results
FILESCOUNT=`ls -1 | grep png | wc -l`

echo "Converting output to png..."
convert result.avs "resultado$FILESCOUNT.png"
echo "Here's your image, have a great day! :)"
eog "resultado$FILESCOUNT.png"
