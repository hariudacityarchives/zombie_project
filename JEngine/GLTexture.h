#pragma once
#include <GL/glew.h>

namespace JEngine {
struct GLTexture {
public:
  GLuint id;
  int width;
  int height;
};
} // namespace JEngine