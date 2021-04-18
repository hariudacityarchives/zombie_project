#include "InputManager.h"

namespace JEngine {
InputManager::InputManager() : _mouseCoords(0.0f) {}

InputManager::~InputManager() {}

void InputManager::update() {
  // for (auto it = _keyMap.begin(); it != _keyMap.end(); it++)
  for (auto &it : _keyMap) {
    _prevKeyMap[it.first] = it.second;
  }
}

void InputManager::pressKey(unsigned int keyID) { _keyMap[keyID] = true; }

void InputManager::releaseKey(unsigned int keyID) { _keyMap[keyID] = false; }

void InputManager::setMouseCoords(float x, float y) {
  _mouseCoords.x = x;
  _mouseCoords.y = y;
}

bool InputManager::isKeyDown(unsigned int keyID) {
  // don't use return _keyMap[keyID], because it may not exist (yet)!
  // so it'll create it and give it some value
  auto it = _keyMap.find(keyID);

  /*if (it != _keyMap.end())
  {
          return it->second;
  }
  else
  {
          return false;
  }*/

  return it == _keyMap.end() ? false : it->second;
}

bool InputManager::isKeyPressed(unsigned int keyID) {
  return isKeyDown(keyID) && !wasKeyPressed(keyID);
}

bool InputManager::wasKeyPressed(unsigned int keyID) {
  auto it = _prevKeyMap.find(keyID);

  return it == _prevKeyMap.end() ? false : it->second;
}
} // namespace JEngine