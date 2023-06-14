#pragma once
#include <cstring>

#include "Matrix4x4f.h"
#include "Collider.h"
using namespace Matrix4x4f;

class Camera {
public:
    Vector4f_t pos;
    Vector4f_t rot;
    Vector4f_t lookAt;
    Vector4f_t orientation;
    Vector4f_t radius;

    Collider* coll;

    float yFov;
    float zNear;
    float zFar;
    float aspectRatio;

    Camera(Vector4f_t lookAt, Vector4f_t position, Vector4f_t rotation, Vector4f_t orientation,
        float yFov, float zNear, float zFar, float aspectRatio);
    Matrix4x4f_t look_At();
    Matrix4x4f_t perspective();
    void move(double timeStep);
};