#include "Object3D.h"

void Object3D::createTriangle() {
    position = Vector4f::make_vector4f(0.0f, 0.0f, 0.0f, 1.0f);
    rotation = Vector4f::make_vector4f(0.0f, 0.0f, 0.0f, 1.0f);
    scale = Vector4f::make_vector4f(1.0f, 1.0f, 1.0f, 1.0f);
    modelMatrix = Matrix4x4f::make_identityf();

    Vertex v1, v2, v3;
    v1.posicion = Vector4f::make_vector4f(0.0f, 1.0f, 0.0f, 1.0f);
    v2.posicion = Vector4f::make_vector4f(-1.0f, -1.0f, 0.0f, 1.0f);
    v3.posicion = Vector4f::make_vector4f(1.0f, -1.0f, 0.0f, 1.0f);

    vertexList.push_back(v1);
    vertexList.push_back(v2);
    vertexList.push_back(v3);

    idList.push_back(0);
    idList.push_back(1);
    idList.push_back(2);
}

void Object3D::updateModelMatrix() {
    Matrix4x4f::Matrix4x4f_t translateMatrix =
        Matrix4x4f::make_translate(position.x, position.y, position.z);
    Matrix4x4f::Matrix4x4f_t rotateMatrix =
        Matrix4x4f::make_rotate(rotation.x, rotation.y, rotation.z);
    Matrix4x4f::Matrix4x4f_t scaleMatrix = Matrix4x4f::make_scale(scale.x, scale.y, scale.z);

    modelMatrix = mult_matrix(translateMatrix, mult_matrix(rotateMatrix, scaleMatrix));
}

void Object3D::loadFromFile(const char* fileName) {
    std::ifstream f(fileName, std::ios_base::in);  // abrir fichero en modo lectura
    if (!f.is_open())
        std::cout << "Error fichero " << fileName << " no encontrado\n";
    else {
        std::cout << "Fichero " << fileName << " encontrado\n";
        idList.clear();
        vertexList.clear();

        int numFaces, numVert;
        f >> numFaces >> numVert;

        for (int i = 0; i < numFaces; i++) {
            int id1, id2, id3;
            f >> id1 >> id2 >> id3;  // leeme de un string y guardalo en un int

            idList.push_back(id1);
            idList.push_back(id2);
            idList.push_back(id3);
        }
        for (int i = 0; i < numVert; i++) {
            // CARGAR TEXTURAS Y NORMALES
            Vertex v;
            f >> v.posicion.x >> v.posicion.y >> v.posicion.z >> v.color.r >> v.color.g >> v.color.b >> v.color.a >> v.normal.x >> v.normal.y >> v.normal.z >> v.uv.x >> v.uv.y;
            v.posicion.w = 1.0f;
            v.normal.w = 0.0f;

            vertexList.push_back(v);
        }

        std::string vshader, fshader, texture;
        f >> vshader;
        f >> fshader;

        prg = new Program();
        prg->addShader(vshader);
        prg->addShader(fshader);
        prg->linkProgram();
        f >> texture;
        mat = new Material(texture);

        this->coll = new sphere();
        for (auto v : this->vertexList)
        {
            particle part{
                make_vector4f(v.posicion.x + 0.1f,v.posicion.y + 0.1f,v.posicion.z + 0.1f,1.0f),

                make_vector4f(v.posicion.x - 0.1f,v.posicion.y - 0.1f,v.posicion.z - 0.1f,1.0f)

            };
            this->coll->addParticle(part);
        }
    }
}

void Object3D::move(double timeStep) {
    if (InputManager::keysState[GLFW_KEY_D]) {
        position.x += 0.001f * timeStep;
    }
    if (InputManager::keysState[GLFW_KEY_A]) {
        position.x -= 0.001f * timeStep;
    }

    if (InputManager::keysState[GLFW_KEY_UP]) {
        scale.x += 0.001f * timeStep;
        scale.y += 0.001f * timeStep;
        scale.z += 0.001f * timeStep;
    }
    if (InputManager::keysState[GLFW_KEY_DOWN]) {
        scale.x -= 0.001f * timeStep;
        scale.y -= 0.001f * timeStep;
        scale.z -= 0.001f * timeStep;
    }

    if (InputManager::keysState[GLFW_KEY_Q]) {
        rotation.y -= 0.1f * timeStep;
    }

    if (InputManager::keysState[GLFW_KEY_E]) {
        rotation.y += 0.1f * timeStep;
    }

    updateModelMatrix();
}