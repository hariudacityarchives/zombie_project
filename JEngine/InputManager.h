#pragma once

#include <glm/glm.hpp>
#include <unordered_map>


namespace JEngine {
class InputManager {
public:
  InputManager();
  ~InputManager();

  void update();

  void pressKey(unsigned int keyID);
  void releaseKey(unsigned int keyID);

  void setMouseCoords(float x, float y);

  bool isKeyDown(unsigned int keyID);
  bool isKeyPressed(unsigned int keyID);
  bool wasKeyPressed(unsigned int keyID);

  // getters

  // const means that this function doesn't do anything to this class itself
  glm::vec2 getMouseCoords() const { return _mouseCoords; }

private:
  std::unordered_map<unsigned int, bool> _keyMap;
  std::unordered_map<unsigned int, bool> _prevKeyMap;
  glm::vec2 _mouseCoords;
};
} // namespace JEngine