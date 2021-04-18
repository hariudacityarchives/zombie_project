#include "Human.h"

#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <random>

#include "../JEngine/ResourceManager.h"

Human::Human() : _frames(0) { _health = 35.0f; }

Human::~Human() {}

void Human::init(float speed, glm::vec2 pos) {
  static std::mt19937 randomEngine(time(nullptr));
  static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

  _color.setColor(255, 255, 255);

  _speed = speed;
  _pos = pos;
  _direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));

  _direction = glm::normalize(_direction);

  _textureID = JEngine::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string> &levelData,
                   std::vector<Human *> &humans, std::vector<Zombie *> &zombies,
                   float deltaTime) {
  static std::mt19937 randomEngine(time(nullptr));
  static std::uniform_real_distribution<float> randomRotate(-40.0f, 40.0f);

  _pos += _direction * _speed * deltaTime;

  if (_frames == 20) {
    _direction = glm::rotate(_direction, randomRotate(randomEngine));
    _frames = 0;
  } else {
    _frames++;
  }

  if (collideWithLevel(levelData)) {
    _direction = glm::rotate(_direction, randomRotate(randomEngine));
    _frames = 0;
  }
}
