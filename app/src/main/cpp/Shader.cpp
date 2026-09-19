#include "Shader.h"
#include <android/log.h>
#include <vector>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "ParkurGame", __VA_ARGS__)

GLuint Shader::compileStage(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len > 0 ? len : 512);
        glGetShaderInfoLog(shader, (GLsizei)log.size(), nullptr, log.data());
        LOGE("Shader derleme hatasi: %s", log.data());
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool Shader::compile(const char* vertexSrc, const char* fragmentSrc) {
    GLuint vs = compileStage(GL_VERTEX_SHADER, vertexSrc);
    GLuint fs = compileStage(GL_FRAGMENT_SHADER, fragmentSrc);
    if (!vs || !fs) return false;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len > 0 ? len : 512);
        glGetProgramInfoLog(program, (GLsizei)log.size(), nullptr, log.data());
        LOGE("Program link hatasi: %s", log.data());
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return true;
}

void Shader::use() const { glUseProgram(program); }

void Shader::setMat4(const char* name, const Mat4& m) const {
    GLint loc = glGetUniformLocation(program, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, m.m);
}

void Shader::setMat3(const char* name, const Mat3& m) const {
    GLint loc = glGetUniformLocation(program, name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, m.m);
}

void Shader::setVec3(const char* name, const Vec3& v) const {
    GLint loc = glGetUniformLocation(program, name);
    glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::setFloat(const char* name, float f) const {
    GLint loc = glGetUniformLocation(program, name);
    glUniform1f(loc, f);
}
