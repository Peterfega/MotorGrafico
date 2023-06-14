#pragma once
#include <vector>

#include "InputManager.h"
#include "Matrix4x4f.h"
#include "Vector4f.h"
#include "Vertex.h"

#include "Collider.h"
#include "Material.h"
#include "Program.h"
#define GLAD_ONLY_HEADERS
#include "common.h"

class Object3D {
   public:
    int ObjectID;
    Program* prg;
    Material* mat;
    Collider* coll;
    Vector4f_t position, rotation, scale;
    Matrix4x4f::Matrix4x4f_t modelMatrix;
    std::vector<Vertex> vertexList;
    std::vector<unsigned int> idList;
    void createTriangle();
    void move(double timeStep);
    void updateModelMatrix();
    void loadFromFile(const char* fileName);
};