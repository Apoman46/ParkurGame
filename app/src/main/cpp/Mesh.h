#pragma once
#include <GLES3/gl3.h>

// Basit kutu (box) mesh'i: pozisyon(3) + normal(3) interleaved VBO, EBO ile indeksli çizim.
class Mesh {
public:
    void createUnitBox();
    void draw() const;
    void destroy();

private:
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLsizei indexCount = 0;
};
