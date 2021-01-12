#pragma once
#ifndef DAELOAD_H
#define DAELOAD_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "tinyxml2.h"

struct Point3D
{
    int x;
    int y;
    int z;
};

class Object3D
{
public:
    Object3D(){};
    ~Object3D(){};

private:
    std::vector<Point3D> vertices;
    std::vector<Point3D> normal;
};

class daeloader
{
public:
    daeloader();
    ~daeloader();

    void import(std::string path);

    std::vector<float> indices;

private:
    bool parse(tinyxml2::XMLNode* parent);

    std::vector<Object3D> objects;
};

#endif