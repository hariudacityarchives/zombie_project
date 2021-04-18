#include "Zombie.h"
#include "Human.h"

#include "../JEngine/ResourceManager.h"

Zombie::Zombie() { _health = 90.0f; }

Zombie::~Zombie() {}

void Zombie::init(float speed, glm::vec2 pos) {
  _speed = speed;
  _pos = pos;
  _color.setColor(255, 255, 255);
  _textureID = JEngine::ResourceManager::getTexture(
                   "../graphicsStuff/Textures/zombie.png")
                   .id;
}

void Zombie::update(const std::vector<std::string> &levelData,
                    std::vector<Human *> &humans,
                    std::vector<Zombie *> &zombies, float deltaTime) {
  Human *closestHuman = getNearestHuman(humans);

  if (closestHuman != nullptr) {
    _direction = glm::normalize(closestHuman->getPosition() - _pos);
    _pos += _direction * _speed * deltaTime;
  }

  collideWithLevel(levelData);
}

Human *Zombie::getNearestHuman(std::vector<Human *> &humans) {
  Human *closestHuman = nullptr;
  float smallestDistance = 0xffffff;

  for (int i = 0; i < humans.size(); i++) {
    glm::vec2 distVec = humans[i]->getPosition() - _pos;
    float distance = glm::length(distVec);

    if (distance < smallestDistance) {
      smallestDistance = distance;
      closestHuman = humans[i];
    }
  }

  return closestHuman;
}