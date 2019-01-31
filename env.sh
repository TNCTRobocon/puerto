#!/bin/bash
LOCAL=${HOME}/local
THREAD=-j4
mkdir ${LOCAL} -p
# install by apt
apt update
apt upgrade -y
apt install -y git  gcc g++  cmake libzmq3-dev libboost-dev 

#install cppzmq
if [ ! -d ${LOCAL}/cppzmq ]; then
    cd ${LOCAL}
    git clone https://github.com/zeromq/cppzmq.git
    cd cppzmq
    mkdir build -p;cd build
    cmake ..
    make install ${THREAD}
fi