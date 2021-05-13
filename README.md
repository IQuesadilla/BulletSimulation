# BulletSimulation

## VCPKG Dependancies:
bullet3
SDL2*
tinyxml2
jsoncpp
glm
tinyobjloader

## .pro Edits
INCLUDEPATH += . ./class ./include ./deps/include ./deps/include/bullet
LIBS += -L./deps/lib -lBulletSoftBody -lBulletDynamics -lBullet3Common -lBulletCollision -lBulletInverseDynamics -lLinearMath -ljsoncpp -ltinyobjloader -ltinyxml2 -ldl -lm
QT += core gui opengl widgets

## Installation
./configure
make
make install*

*doesn't work right now