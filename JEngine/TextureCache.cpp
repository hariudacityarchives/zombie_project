#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace JEngine {
TextureCache::TextureCache() {}

TextureCache::~TextureCache() {}

GLTexture TextureCache::getTexture(std::string texturePath) {
  // lookup the texture and see if its in the map
  /*std::map<std::string, GLTexture>::iterator*/ auto mit =
      _textureMap.find(texturePath);

  // oh noes! It's not in the map
  if (mit == _textureMap.end()) {
    // no error checking needed
    GLTexture newTexture = ImageLoader::loadPNG(texturePath);

    // Insert it into the map
    _textureMap.insert(make_pair(texturePath, newTexture));

    // std::cout << "Added texture " << texturePath << "!" << std::endl;

    return newTexture;
  }

  // std::cout << "Loaded texture!" << std::endl;

  // return second
  return mit->second;
}
} // namespace JEngine