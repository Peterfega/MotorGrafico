#pragma once
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Object3D.h"
#define GLAD_ONLY_HEADERS
#include "common.h"

typedef struct {
    unsigned int buffId;
    unsigned int vertId;
    unsigned int listId;
} BufferObject;

class Render {
public:
    Render(int width, int height);
    ~Render();
    void PutPixel(int x, int y);
    void resetBuffer();
    void Draw();
    void initGL();
    void putObject(Object3D* obj);
    void putCamera(Camera* camj);
    void drawGL();
    void mainLoop();
    void setupObject(Object3D* obj);
    void putLight(Light* light);
    Light* getLight(unsigned int i);

    inline static std::vector<Object3D*> objectList;

private:
    int bufferWidth;
    int bufferHeight;
    char** buffer;
    //Light* light;
    GLFWwindow* window;
    std::vector<Light*> lights;
    Camera* cam;
    std::map<int, BufferObject> bufferList;
};