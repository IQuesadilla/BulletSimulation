#!/bin/sh

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

./vcpkg install bullet3
./vcpkg install tinyxml2
./vcpkg install jsoncpp
./vcpkg install glm
./vcpkg install tinyobjloader

cd ..
qmake
make