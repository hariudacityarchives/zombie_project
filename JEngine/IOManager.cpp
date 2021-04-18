#include "IOManager.h"

#include <fstream>

namespace JEngine {
bool IOManager::readFileToBuffer(std::string filePath,
                                 std::vector<unsigned char> &buffer) {
  std::ifstream file(filePath, std::ios::binary);
  if (file.fail()) {
    perror(filePath.c_str());
    return false;
  }

  // seek to the end of the file
  file.seekg(0, std::ios::end);

  // returns how many bytes there are before the current location(the end, so
  // the fileSize)
  int fileSize = file.tellg();
  // go back to the beginning
  file.seekg(0, std::ios::beg);

  // just to be safe.
  fileSize -= file.tellg();

  buffer.resize(fileSize);

  // first element of the buffer adress
  // TRICK C++! mUHAHHAHAHAH
  file.read((char *)&(buffer[0]), fileSize);
  file.close();

  return true;
}
} // namespace JEngine
