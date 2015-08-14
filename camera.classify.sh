#!/bin/bash

set -e -x
while true
do
        ./picture.face.bin
        WC=`pwd`
        Pictue=${WC}/face.classify.png
        if [ -f ${WC}/face.classify.png ] ; then
	        cd /home/yma/workspace/caffe-dev/waffe && \
	        ./examples/facerecognition/classification.sh \
	        ${Pictue}
	        cd ${WC}
        fi
        read -p "Press [Enter] key to resume."
done


