#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

static std::string readFile(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Shader] Cannot open: " << path << "\n";
        return "";
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertSrc = readFile(vertexPath);
    std::string fragSrc = readFile(fragmentPath);

    GLuint vert = compile(vertSrc.c_str(), GL_VERTEX_SHADER);
    GLuint frag = compile(fragSrc.c_str(), GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);
    glLinkProgram(ID);

    int ok;
    glGetProgramiv(ID, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(ID, 512, nullptr, log);
        std::cerr << "[Shader] Link error:\n" << log << "\n";
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

GLuint Shader::compile(const char* src, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "[Shader] Compile error ("
                  << (type == GL_VERTEX_SHADER ? "vert" : "frag") << "):\n"
                  << log << "\n";
    }
    return shader;
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setMat4(const std::string& name, const glm::mat4& v) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
}
void Shader::setMat3(const std::string& name, const glm::mat3& v) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
}
void Shader::setVec3(const std::string& name, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v));
}
void Shader::setFloat(const std::string& name, float v) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), v);
}
