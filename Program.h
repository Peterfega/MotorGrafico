#pragma once
#define GLAD_ONLY_HEADERS
#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "common.h"
class Program {
   public:
    unsigned int idProgram = -1;
    std::vector<Shader*> shaderList;
    std::map<std::string, unsigned int> varList;

    Program() {
        idProgram = glCreateProgram();
    }

    void addShader(std::string fileName) {
        Shader* sh = new Shader(fileName);
        shaderList.push_back(sh);
    }

    void linkProgram() {
        for (auto i : shaderList) {
            glAttachShader(idProgram, i->idShader);
        }
        glLinkProgram(idProgram);

        checkErrors();
        readVarList();
    }

    void checkErrors() {
        GLint program_linked;

        glGetProgramiv(idProgram, GL_LINK_STATUS, &program_linked);

        if (program_linked != GL_TRUE) {
            GLsizei log_length = 0;
            GLchar message[1024];

            glGetProgramInfoLog(idProgram, 1024, &log_length, message);
            std::cout << "ERROR \n"
                      << message << "\n\n";
        }
    }

    void clean() {
        for (auto shader : shaderList) {
            delete shader;
        }
        glDeleteProgram(idProgram);
    }

    void readVarList() {
        int numAttributes = 0;
        int numUniforms = 0;

        glGetProgramiv(idProgram, GL_ACTIVE_ATTRIBUTES, &numAttributes);
        for (int i = 0; i < numAttributes; i++) {
            char varName[100];
            int bufSize = 100, length = 0, size = 0;
            GLenum type = -1;
            glGetActiveAttrib(idProgram, (GLuint)i, bufSize, &length, &size, &type, varName);
            varList[std::string(varName)] = glGetAttribLocation(idProgram, varName);
        }

        glGetProgramiv(idProgram, GL_ACTIVE_UNIFORMS, &numUniforms);
        for (int i = 0; i < numUniforms; i++) {
            char varName[100];
            int bufSize = 100, length = 0, size = 0;
            GLenum type = -1;
            glGetActiveUniform(idProgram, (GLuint)i, bufSize, &length, &size, &type, varName);
            varList[std::string(varName)] = glGetUniformLocation(idProgram, varName);
        }
    }

    void use() {
        glUseProgram(idProgram);
    }

    void setAttribute(int location, void* description) {
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), description);
    }

    void setUniformMatrix4x4(unsigned int location, float* matrix) {
        glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
    }

    void setVertexPositions(void* description) {
        setAttribute(getVarLocation("pos"), description);
    }

    void setVertexColors(void* description) {
        setAttribute(getVarLocation("color"), description);
    }

    void setVertexNormals(void* description) {
        setAttribute(getVarLocation("normal"), description);
    }

    void setVertexUVs(void* description) {
        setAttribute(getVarLocation("uv"), description);
    }

    void setMatrixMVP(float* matrix) {
        setUniformMatrix4x4(getVarLocation("MVP"), matrix);
    }

    void setMatrixM(float* matrix) {
        setUniformMatrix4x4(getVarLocation("M"), matrix);
    }

    unsigned int getVarLocation(std::string varName) {
        if (varList.find(varName) != varList.end()) {
            return varList[varName];
        } else {
            std::cout << "ERROR: variable " << varName << " not found in shader!\n";
            return -1;
        }
    }

    void setLight(Light& light) {
        glUniform4fv(getVarLocation("light.position"), 1, (float*)&light.position.x);
        glUniform4fv(getVarLocation("light.color"), 1, (float*)&light.color.x);
        glUniform4fv(getVarLocation("light.direction"), 1, (float*)&light.direction.x);
        glUniform1i(getVarLocation("light.type"), light.type);
        glUniform1f(getVarLocation("light.ka"), light.ka);
        glUniform1f(getVarLocation("light.kd"), light.kd);
        glUniform1f(getVarLocation("light.ks"), light.ks);
    }

    void setMaterial(Material& mat, unsigned int textureUnitId) {
        glUniform1i(getVarLocation("materials.textureUnitID"), textureUnitId);
        // glUniform1i(getVarLocation("materials.enabled"), mat.enabled);
    }
};