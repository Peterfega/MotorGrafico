#include "Shader.h"

Shader::Shader(std::string filename) {
    this->fileName = filename;
    if (fileName.ends_with(".vertex")) {
        this->type = GL_VERTEX_SHADER;
    }
    if (fileName.ends_with(".fragment")) {
        this->type = GL_FRAGMENT_SHADER;
    }
    idShader = glCreateShader(this->type);
    readSource();
    compileShader();
    // clean(); Cuidado que si se limpia hay error, creo q hay q invocarlo desde program
}

void Shader::readSource() {
    std::ifstream f(fileName);
    if (f.is_open()) {
        source = std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    }
    else {
        checkErrors();
    }
}

void Shader::compileShader() {
    const char* src = source.c_str();
    glShaderSource(idShader, 1, &src, nullptr);
    glCompileShader(idShader);
}

void Shader::checkErrors() {
    std::cout << "ERROR: FILE " << fileName << " NOT FOUND IN " << __FILE__ << ": "
        << __LINE__ << std::endl;
}

void Shader::clean() {
    glDeleteShader(idShader);
}
