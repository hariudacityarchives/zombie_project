#include "Camera2D.h"

#include <algorithm>

namespace JEngine {
Camera2D::Camera2D()
    : _scale(1.0f), _position(0.0f, 0.0f), _cameraMatrix(1.0f),
      _orthoMatrix(1.0f), _needsMatrixUpdate(true), _screenWidth(500),
      _screenHeight(500) // <-- force init
{}

Camera2D::~Camera2D() {}

void Camera2D::init(int screenWidth, int screenHeight) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;

  // build ortho matrix
  // pos offset left, width, pos offset bottom, screenheight
  _orthoMatrix =
      glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
}

void Camera2D::update() {
  if (_needsMatrixUpdate) {
    glm::vec3 translate(-_position.x + _screenWidth / 2,
                        -_position.y + _screenHeight / 2, 0.0f); // not 3D :P

    // convert to screen coords
    _cameraMatrix = glm::translate(_orthoMatrix, translate);
    glm::vec3 scale(_scale, _scale, 0.0f);
    _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

    _needsMatrixUpdate = false;
  }
}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
  // invert Y direction
  screenCoords.y = _screenHeight - screenCoords.y;

  // ZERO is center! Make it that way!
  screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);

  // Then take the translation into account

  // zoom in --> coords smaller
  // zoom out --> coords greater
  screenCoords /= _scale;

  // move camera to the left, x coord is decreasing, so:
  screenCoords += _position;

  // ORDER MATTERS!!
  return screenCoords;
}

bool Camera2D::isBoxInView(const glm::vec2 &pos, const glm::vec2 &dim) {
  glm::vec2 scaleDim = glm::vec2(_screenWidth, _screenHeight) / _scale;

  const float MIN_DISTANCE_X = dim.x / 2.0f + scaleDim.x / 2.0f;
  const float MIN_DISTANCE_Y = dim.y / 2.0f + scaleDim.y / 2.0f;

  glm::vec2 centerPos = pos + dim / 2.0f;
  glm::vec2 distVec = centerPos - _position; // position is already center

  float xDepth = MIN_DISTANCE_X - abs(distVec.x);
  float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

  return xDepth > 0 && yDepth > 0;
}
} // namespace JEngine