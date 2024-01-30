#!/bin/bash

rootDir=$(git rev-parse --show-toplevel)
compileCommandDir=$rootDir/build/debug

run-clang-tidy -p $compileCommandDir \
    $rootDir/Main.cpp \
    $rootDir/include/*.hpp \
    $rootDir/include/Poller/*.hpp \
    $rootDir/src/*.cpp \
    $rootDir/src/Poller/*.cpp \
    -quiet
