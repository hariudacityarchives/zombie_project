#pragma once

#include "GLTexture.h"

#include <string>

namespace JEngine {
class ImageLoader {
public:
  static GLTexture loadPNG(std::string filePath);
};
} // namespace JEngine