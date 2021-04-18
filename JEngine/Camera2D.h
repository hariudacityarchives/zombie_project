#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace JEngine {
class Camera2D {
public:
  Camera2D();
  ~Camera2D();

  void update();

  glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

  bool isBoxInView(const glm::vec2 &pos, const glm::vec2 &dim);

  void init(int screenWidth, int screenHeight);

  // setters
  void setPosition(glm::vec2 &&pos) {
    _position = pos;
    _needsMatrixUpdate = true;
  }
  void setScale(float scale) {
    _scale = scale;
    _needsMatrixUpdate = true;
  }

  // getters
  glm::vec2 getPosition() { return _position; }
  float getScale() { return _scale; }
  glm::mat4 getCameraMatrix() { return _cameraMatrix; }

private:
  int _screenWidth, _screenHeight;
  bool _needsMatrixUpdate;
  float _scale;
  glm::vec2 _position;
  glm::mat4 _cameraMatrix; // 4 by 4 matrix
  glm::mat4 _orthoMatrix;
};
} // namespace JEngine