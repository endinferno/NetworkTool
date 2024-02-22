#!/bin/bash

PrintWithColor() {
    echo -e "$1$2\e[0m\c"
}

PrintBuildType() {
    echo -e "Compiling \c"
    PrintWithColor "\e[31m" $BUILD_TYPE
    echo " Version"
}

CMakePhase() {
    PrintWithColor "\e[32m" "CMake Phase"
    echo ""
    pushd $BUILD_DIR > /dev/null

    cmake -B $BUILD_PATH \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

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

BUILD_DIR=$(pwd)/build
# Create build directory
if [ ! -d $BUILD_DIR ];
then
    mkdir $BUILD_DIR
fi

# Read Build Type
BUILD_TYPE=debug
if [ $# -eq 1 ];
then
    GetBuildType $1
fi

BUILD_PATH=${BUILD_TYPE}
BUILD_TYPE=${BUILD_TYPE^}

PrintBuildType
CMakePhase
MakePhase
