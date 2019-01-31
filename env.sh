#!/bin/bash
LOCAL=${HOME}/local
THREAD=-j4

# install 
apt update
apt upgrade -y
apt install git cmake gcc g++ -y

mkdir ${LOCAL} -p
# install boost
if [ ! -d ${LOCAL}/boost ]; then 

    cd ${LOCAL}
    mkdir boost.tmp -p;cd boost.tmp
    # おとしてして解凍
    if [ ! -d boost_1_69_0 ];then
        wget https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.gz
        tar -xzvf boost_1_69_0.tar.gz
    fi
    cd boost_1_69_0
    if [ ! -f b2 ]; then
        ./bootstrap.sh
    fi
    mkdir ${HOME}/local/boost -p
    ./b2 install ${THREAD} --prefix=${HOME}/local/boost 
fi

# install libzmq
if [ ! -d ${LOCAL}/libzmq ]; then
    cd ${LOCAL}
    git clone https://github.com/zeromq/libzmq.git
    cd libzmq;mkdir build -p;cd build
    cmake ..
    make install ${THREAD}
fi
#install cppzmq
if [ ! -d ${LOCAL}/cppzmq ]; then
    cd ${LOCAL}
    git clone https://github.com/zeromq/cppzmq.git
    cd cppzmq
    mkdir build -p;cd build
    cmake ..
    make install ${THREAD}
fi