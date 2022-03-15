#!/bin/bash
# run the Makefile
make

# this ip is the docker machine ip docker0: see the value using
# ifconfig
# $1 = IP
# $2 = PORT
# $2 = pattern
./video_sample $1 $2 $3