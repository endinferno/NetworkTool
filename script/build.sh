#!/bin/bash

PrintWithColor() {
    echo -e "$1$2\e[0m\c"
}

PrintBuildType() {
    echo -e "Compiling \c"
    PrintWithColor "\e[31m" $BUILD_TYPE
    echo " Version"
}

PrintPollType() {
    echo -e "Compiling with \c"
    PrintWithColor "\e[31m" $POLL_TYPE
    echo " Poll Type"
}

CMakePhase() {
    PrintWithColor "\e[32m" "CMake Phase"
    echo ""
    pushd $BUILD_DIR > /dev/null

    cmake -B $BUILD_PATH \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
        -DPOLL_TYPE=$POLL_TYPE ..

    popd > /dev/null
}

MakePhase() {
    PrintWithColor "\e[32m" "Make Phase"
    echo ""
    pushd $BUILD_DIR/$BUILD_PATH > /dev/null
    make -j12
    popd > /dev/null
}

GetBuildType() {
    BUILD_PARAM=$1
    BUILD_TYPE=${BUILD_PARAM,,}
    if [ $BUILD_TYPE != "debug" ] && [ $BUILD_TYPE != "release" ];
    then
        BUILD_TYPE=debug
    fi
}

GetPollType() {
    POLL_PARAM=$1
    POLL_TYPE=${POLL_PARAM,,}
    if [ $POLL_TYPE != "epoll" ] && [ $POLL_TYPE != "poll" ] && [ $POLL_TYPE != "select" ];
    then
        POLL_TYPE=epoll
    fi
}

BUILD_DIR=$(pwd)/build
# Create build directory
if [ ! -d $BUILD_DIR ];
then
    mkdir $BUILD_DIR
fi

# Read Build Type
BUILD_TYPE=debug
POLL_TYPE=epoll
if [ $# -eq 1 ];
then
    GetBuildType $1
elif [ $# -eq 2 ];
then
    GetBuildType $1
    GetPollType $2
fi

BUILD_PATH=${BUILD_TYPE}
BUILD_TYPE=${BUILD_TYPE^}
POLL_TYPE=${POLL_TYPE^}

PrintBuildType
PrintPollType
CMakePhase
MakePhase
