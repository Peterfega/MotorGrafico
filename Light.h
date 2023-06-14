#pragma once
#define GLAD_ONLY_HEADERS
#include "Matrix4x4f.h"
#include "common.h"

class Light {
public:
    typedef enum {
        directional = 0,
        puntual = 1
    } lightTypes;

    Light(Vector4f_t position, Vector4f_t color, Vector4f_t direction, int type, float ka, float kd, float ks);

    void move(double timeStep);
    Vector4f_t position;
    Vector4f_t color;
    Vector4f_t direction;
    float ka;
    float kd;
    float ks;
    int type;
};