#pragma once
#include "GLTexture.h"
#include <GL/glew.h>


#include <string>

namespace JEngine {
class Sprite {
public:
  Sprite();
  ~Sprite();

  void init(float x, float y, float width, float height,
            std::string texturePath);
  void draw();

private:
  float _x;
  float _y;
  float _width;
  float _height;

  GLuint _vboId; // vertex buffer object ID

  GLTexture _texture;
};
} // namespace JEngine