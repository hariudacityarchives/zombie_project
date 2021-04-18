#include "Sprite.h"
#include "ResourceManager.h"
#include "Vertex.h"


#include <cstddef>
#include <iostream>


namespace JEngine {
Sprite::Sprite() : _vboId(0) {}

Sprite::~Sprite() {
  // destroy the buffer, preventing memory leaks!
  if (_vboId != 0) {
    glDeleteBuffers(1, &_vboId);
  }
}

void Sprite::init(float x, float y, float width, float height,
                  std::string texturePath) {
  _x = x;
  _y = y;
  _width = width;
  _height = height;

  _texture = ResourceManager::getTexture(texturePath);

  // VBO(vertex buffer object) hasn't been initialized
  if (_vboId == 0) {
    // std::cout << "Generating VBO @ 0x" << &_vboId << "..." << std::endl;
    glGenBuffers(
        1, &_vboId); // give the vbo pointer to opengl, it'll do magic stuff
    // std::cout << "Generated, VBOID = " << _vboId << std::endl;
  }

  // 2 triangles --> 6 vertexes --> 6*2 = 12 locations
  Vertex vertexData[6];

  // first triangle
  vertexData[0].setPosition(x + width, y + height);
  vertexData[0].setUV(1.0f, 1.0f);

  vertexData[1].setPosition(x, y + height);
  vertexData[1].setUV(0.0f, 1.0f);

  vertexData[2].setPosition(x, y);
  vertexData[2].setUV(0.0f, 0.0f);

  // second triangle
  vertexData[3].setPosition(x, y);
  vertexData[3].setUV(0.0f, 0.0f);

  vertexData[4].setPosition(x + width, y);
  vertexData[4].setUV(1.0f, 0.0f);

  vertexData[5].setPosition(x + width, y + height);
  vertexData[5].setUV(1.0f, 1.0f);

  // Set all vertex colors to magenta
  for (int i = 0; i < 6; i++) {
    vertexData[i].setColor(255, 0, 255, 255);
  }

  // vertexData[1].setColor(0, 0, 255, 255);

  // vertexData[4].setColor(0, 255, 0, 255);

  // bind buffer
  glBindBuffer(GL_ARRAY_BUFFER, _vboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  // Unbind buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw() {
  glBindTexture(GL_TEXTURE_2D,
                _texture.id); // don't unbind, I might need it later

  glBindBuffer(GL_ARRAY_BUFFER, _vboId);

  // send one vbo(index)
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // 0 index, 2 elements(x, y), mode, don't normalize(convert it to range 0 and
  // 1), size of the struct, offset(it's zero, but maybe later not?) position
  // attribute pointer
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, position));

  // color attribute color
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                        (void *)offsetof(Vertex, color));

  // UV attribute pointer
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, uv));

  // mode, elementStartArray, how many vertexes
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
} // namespace JEngine