#include "ImageLoader.h"
#include "Errors.h"
#include "IOManager.h"
#include "PicoPNG.h"

namespace JEngine {
GLTexture ImageLoader::loadPNG(std::string filePath) {
  GLTexture texture = {}; // will init everything to zero

  std::vector<unsigned char> in;  // input
  std::vector<unsigned char> out; // output

  // OH SHIZ!! ERORRZ!!
  if (!IOManager::readFileToBuffer(filePath, in)) {
    fatalError("Failed to load PNG file to buffer!");
  }

  unsigned long width, height;

  int errorCode = decodePNG(out, width, height, &(in[0]), in.size());

  if (errorCode != 0) {
    fatalError("decodePNG failed with error(failed to decode PNG): " +
               std::to_string(errorCode));
  }

  glGenTextures(1, &(texture.id));

  glBindTexture(GL_TEXTURE_2D, texture.id);

  // texture, mipmap, internal color format, width, height, border, color
  // format, what type of data, output of the decoder
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, &(out[0]));

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrap
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnitude
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);

  texture.width = width;
  texture.height = height;

  return texture;
}
} // namespace JEngine