#pragma once

#include <string>
#include <vector>

namespace JEngine {
class IOManager {
public:
  static bool readFileToBuffer(std::string filePath,
                               std::vector<unsigned char> &buffer);
};
} // namespace JEngine
