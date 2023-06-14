#include <iostream>

#include "common.h"
// #include "InputManager.h"
// #include "Matrix4x4f.h"
#include "Render.h"

int main() {
    Camera* cam =
        new Camera(make_vector4f(0.0f, 0.0f, 0.0f, 1.0f), make_vector4f(0.0f, 0.0f, 3.0f, 1.0f),
                   make_vector4f(0.0f, 0.0f, 0.0f, 1.0f), make_vector4f(0.0f, 1.0f, 0.0f, 0.0f),
                   90.0f, 0.01f, 100.0f, 4.0f / 3.0f);

    Light* light = new Light(make_vector4f(0.0f, 0.0f, -3.0f, 1.0f), make_vector4f(1.0f, 1.0f, 1.0f, 1.0f),
                             normalize(resta_vector(make_vector4f(3.0f, 3.0f, 3.0f, 1.0f), make_vector4f(0.0f, 0.0f, 0.0f, 1.0f))), 1, 0.25f, 1.0f, 1.0f);

    Render render = Render(640, 480);
    render.initGL();

    render.putLight(light);

    render.putCamera(cam);

    Object3D* cube = new Object3D();
    cube->loadFromFile("data/cubeBlendTexture.vertc");
    render.putObject(cube);
    render.setupObject(cube);

    render.mainLoop();

    return 0;
}