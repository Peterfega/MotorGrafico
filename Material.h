#pragma once

#define GLAD_ONLY_HEADERS
#include "Matrix4x4f.h"
#include "common.h"

class Material {
   public:
    std::string textureName;
    unsigned int textureId;
    int enabled;
    int width, height;
    unsigned char* data;
    void materialBind(unsigned int textureUnitId);
    Material(std::string textureName);
    void loadImage();
};
