#!/bin/bash
export CROSS='i586-mingw32msvc-'

./build.sh -prefix 'C:\eneboo' -platform 'win32-g++-cross' -quick && ./dist_windows.sh -quick

