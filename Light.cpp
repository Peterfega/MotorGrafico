#include "Light.h"

Light::Light(Vector4f_t position, Vector4f_t color, Vector4f_t direction, int type, float ka, float kd, float ks) {
    this->direction = direction;
    this->position = position;
    this->color = color;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->type = type;
}

void Light::move(double timeStep) {
    // Implementación del algoritmo de movimiento de la luz
    // Por el momento, vacío
}