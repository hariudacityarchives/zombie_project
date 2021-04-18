#include "ResourceManager.h"

namespace JEngine {
TextureCache ResourceManager::_textureCache;

GLTexture ResourceManager::getTexture(std::string texturePath) {
  return _textureCache.getTexture(texturePath);
}
} // namespace JEngine