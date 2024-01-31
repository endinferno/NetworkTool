#!/bin/bash

rootDir=$(git rev-parse --show-toplevel)

clang-format -style=file -i $rootDir/Main.cpp
find $rootDir/src -regex '.*\.\(cpp\|hpp\)' -exec clang-format -style=file -i {} \;
find $rootDir/include -regex '.*\.\(cpp\|hpp\)' -exec clang-format -style=file -i {} \;
find $rootDir/test -regex '.*\.\(cpp\|hpp\)' -exec clang-format -style=file -i {} \;
