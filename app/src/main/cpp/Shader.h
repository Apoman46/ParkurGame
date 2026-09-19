#pragma once
#include <GLES3/gl3.h>
#include "MathUtil.h"

class Shader {
public:
    GLuint program = 0;

    bool compile(const char* vertexSrc, const char* fragmentSrc);
    void use() const;

    void setMat4(const char* name, const Mat4& m) const;
    void setMat3(const char* name, const Mat3& m) const;
    void setVec3(const char* name, const Vec3& v) const;
    void setFloat(const char* name, float f) const;

private:
    GLuint compileStage(GLenum type, const char* src);
};
