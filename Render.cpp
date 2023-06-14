#include "Render.h"

Render::Render(int width, int height) {
    bufferWidth = width;
    bufferHeight = height;
    buffer = new char*[width];
    for (int i = 0; i < width; i++) {
        buffer[i] = new char[height];
    }
    resetBuffer();
}

Render::~Render() {
    for (int i = 0; i < bufferWidth; i++) {
        delete[] buffer[i];
    }
    delete[] buffer;

    for (int i = 0; i < objectList.size(); i++) {
        delete objectList[i];
    }

    glfwTerminate();
}

void Render::PutPixel(int x, int y) {
    if (x <= ((bufferWidth - 1) / 2) && x >= ((-bufferWidth + 1) / 2) &&
        y <= ((bufferHeight - 1) / 2) && y >= ((-bufferHeight + 1) / 2)) {
        buffer[x + (bufferWidth - 1) / 2][y + (bufferHeight - 1) / 2] = '1';
    }
}

void Render::resetBuffer() {
    for (int x = 0; x < bufferWidth; x++) {
        for (int y = 0; y < bufferHeight; y++) {
            buffer[x][y] = '0';
        }
    }
}

void Render::Draw() {
    for (int y = bufferHeight - 1; y >= 0; y--) {
        for (int x = 0; x < bufferWidth; x++) {
            if (buffer[x][y] == '0') {
                std::cout << " ";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

void Render::initGL() {
    if (!glfwInit()) {
        std::cout << "ERROR: GFWINIT\n";
        return;
    } else {
        std::cout << "GLFW Init!\n";
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(bufferWidth, bufferHeight, "Practica 5", nullptr, nullptr);
    if (!window) {
        std::cerr << "ERROR: Couldn't create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    InputManager::initInputManager(window);
}

void Render::putObject(Object3D* obj) {
    setupObject(obj);
    objectList.push_back(obj);
}

void Render::putCamera(Camera* camj) { cam = camj; }

void Render::drawGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    Matrix4x4f_t viewMatrix = cam->look_At();

    for (int i = 0; i < objectList.size(); i++) {
        Object3D* obj = objectList[i];
        // obj->move(1);
        // obj->updateModelMatrix();
        // Matrix4x4f_t modelMatrix =
        //     make_translate(obj->position.x, obj->position.y, obj->position.z);
        Matrix4x4f_t modelMatrix = mult_matrix(viewMatrix, make_identityf());
        modelMatrix = mult_matrix(cam->perspective(), modelMatrix);

        Matrix4x4f_t MVP = transpose(modelMatrix);

        // Light* light = new Light(make_vector4f(0.0f, 0.0f, -3.0f, 1.0f), make_vector4f(1.0f, 1.0f, 1.0f, 1.0f),
        //     normalize(resta_vector(make_vector4f(3.0f, 3.0f, 3.0f, 1.0f), make_vector4f(0.0f, 0.0f, 0.0f, 1.0f))), 1, 0.25f, 1.0f, 1.0f);

        BufferObject buffer = bufferList[obj->ObjectID];

        glBindVertexArray(buffer.buffId);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.vertId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.listId);

        obj->prg->use();

        obj->prg->setVertexPositions((void*)offsetof(Vertex, posicion));
        //->prg->setVertexColors((void*)offsetof(Vertex, color));
        obj->prg->setVertexNormals((void*)offsetof(Vertex, normal));
        obj->prg->setVertexUVs((void*)offsetof(Vertex, uv));

        obj->prg->setMatrixMVP((float*)&MVP);
        // AQUI ESTA M
        obj->prg->setMatrixM((float*)&modelMatrix);

        // AQUI ESTA EL PROBLEMA
        obj->prg->setLight(*this->getLight(0));

        // obj->prg->addShader("data/shader.vertex");
        // obj->prg->addShader("data/shader.fragment");
        // obj->prg ???

        // A�ADIR FUNCIONES TEXTURAS
        obj->mat->materialBind(0);
        obj->prg->setMaterial(*obj->mat, 0);

        // glUniformMatrix4fv(, 1, GL_FALSE, (float*)&MVP);

        // glPushMatrix();
        // glMultMatrixf((float*)&MVP);
        // glEnableClientState(GL_VERTEX_ARRAY);
        // glVertexPointer(4, GL_FLOAT, sizeof(Vertex), obj->vertexList.data());
        glDrawElements(GL_TRIANGLES, obj->idList.size(), GL_UNSIGNED_INT, nullptr);
        // glPopMatrix();
    }

    glfwSwapBuffers(window);
}

void Render::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        cam->move(1);
        for (int j = 0; j < lights.size(); j++) {
            lights[j]->move(1);
        }
        for (int i = 0; i < objectList.size(); i++) {
            //objectList[i]->move(1);
            objectList[i]->updateModelMatrix();
        }
        drawGL();
    }
}

void Render::setupObject(Object3D* obj) {
    BufferObject buffer;

    glGenVertexArrays(1, &buffer.buffId);
    glGenBuffers(1, &buffer.vertId);
    glGenBuffers(1, &buffer.listId);

    glBindVertexArray(buffer.buffId);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vertId);
    glBufferData(GL_ARRAY_BUFFER, obj->vertexList.size() * sizeof(Vertex), obj->vertexList.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.listId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->idList.size() * sizeof(unsigned int), obj->idList.data(), GL_STATIC_DRAW);
    bufferList[obj->ObjectID] = buffer;
}

void Render::putLight(Light* light) {
    this->lights.push_back(light);
}

Light* Render::getLight(unsigned int i) {
    return this->lights[i];
}