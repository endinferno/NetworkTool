#!/bin/bash

clang-format -style=file -i Main.cpp
find src -regex '.*\.\(cpp\|hpp\)' -exec clang-format -style=file -i {} \;
find include -regex '.*\.\(cpp\|hpp\)' -exec clang-format -style=file -i {} \;
