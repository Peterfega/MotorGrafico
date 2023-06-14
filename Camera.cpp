#include "Camera.h"
#include "Render.h"

#include "InputManager.h"

Camera::Camera(Vector4f_t lookAt, Vector4f_t position, Vector4f_t rotation, Vector4f_t orientation,
    float yFov, float zNear, float zFar, float aspectRatio) {
    this->lookAt = lookAt;
    this->pos = position;
    this->rot = rotation;
    this->orientation = orientation;
    this->radius = resta_vector(lookAt, position);
    this->yFov = yFov;
    this->zNear = zNear;
    this->zFar = zFar;
    this->aspectRatio = aspectRatio;

    //Collider
    this->coll = new sphere();
    particle part{
                make_vector4f(this->pos.x + 0.1f,this->pos.y + 0.1f,this->pos.z + 0.1f,1.0f),

                make_vector4f(this->pos.x - 0.1f,this->pos.y - 0.1f,this->pos.z - 0.1f,1.0f)

    };
    this->coll->addParticle(part);
}

Matrix4x4f_t Camera::look_At() {
    Matrix4x4f_t m = make_identityf();
    Vector4f_t up, forward, right;
    forward = normalize(resta_vector(lookAt, pos));
    forward.w = 0;
    right = normalize(cross_product_vector(orientation, forward));
    right.w = 0;
    up = (cross_product_vector(forward, right));
    up.w = 0;

    m.data2d[0][0] = right.x;
    m.data2d[1][0] = right.y;
    m.data2d[2][0] = right.z;
    m.data2d[0][1] = up.x;
    m.data2d[1][1] = up.y;
    m.data2d[2][1] = up.z;
    m.data2d[0][2] = -forward.x;
    m.data2d[1][2] = -forward.y;
    m.data2d[2][2] = -forward.z;
    m.data2d[3][0] = -dot_product_vector(right, pos);
    m.data2d[3][1] = -dot_product_vector(up, pos);
    m.data2d[3][2] = dot_product_vector(forward, pos);
    m.data2d[3][3] = 1.0f;
    return transpose(m);
}

Matrix4x4f_t Camera::perspective() {
    Matrix4x4f_t m = make_identityf();
    // memset(&m, 0, sizeof(float) * 16);
    float tangent = tan(TO_RADIANS(yFov * 0.5f));

    m.data2d[0][0] = 1.0f / (aspectRatio * tangent);
    m.data2d[1][1] = 1.0f / (tangent);
    m.data2d[2][2] = -(zFar + zNear) / (zFar - zNear);
    m.data2d[2][3] = -2.0f * zFar * zNear / (zFar - zNear);
    m.data2d[3][2] = -1.0f;
    m.data2d[3][3] = 0;

    return m;
}

void Camera::move(double timeStep) {
    // camera_t cam;
    static float prevMX = 0, prevMY = 0;
    
    Vector4f_t prev_pos = pos;

    // Horizontal plane movement
    if (InputManager::keysState[GLFW_KEY_LEFT]) {
        pos.x -= 0.001f;
    }
    if (InputManager::keysState[GLFW_KEY_RIGHT]) {
        pos.x += 0.001f;
    }
    if (InputManager::keysState[GLFW_KEY_UP]) {
        pos.z += 0.001f;
    }
    if (InputManager::keysState[GLFW_KEY_DOWN]) {
        pos.z -= 0.001f;
    }

    float speedX = InputManager::posMX - prevMX;
    float speedY = InputManager::posMY - prevMY;

    rot.x += speedY * 5.0f;
    rot.y += speedX * 5.0f;

    Matrix4x4f_t mRot = make_rotate(rot.x, rot.y, 0.0f);
    lookAt = mult_vector(mRot, radius);
    lookAt = suma_vector(lookAt, pos);

    prevMX = InputManager::posMX;
    prevMY = InputManager::posMY;

    ((sphere*)(this->coll))->center = pos;

    if (this->coll->test(Render::objectList[0]->coll)) {
        pos = prev_pos;
        //((sphere*)(this->coll))->center = pos;
    }
}
