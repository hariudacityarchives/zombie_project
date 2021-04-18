#pragma once
#include "GLTexture.h"
#include <map>


namespace JEngine {
class TextureCache {
public:
  TextureCache();
  ~TextureCache();

  GLTexture getTexture(std::string texturePath);

private:
  std::map<std::string, GLTexture> _textureMap;
};
} // namespace JEngine
