#pragma once

#include "../JEngine/SpriteBatch.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Human;
class Zombie;
class Agent;

class Bullet {
public:
  Bullet(glm::vec2 pos, glm::vec2 dir, float vel, float dmg);
  ~Bullet();

  // on return true, delete bullet
  bool update(const std::vector<std::string> &levelData, float deltaTime);

  void draw(JEngine::SpriteBatch &sb);

  bool collideWithAgent(Agent *agent);

  // getters
  float getDamage() const { return _dmg; }
  glm::vec2 getPosition() const { return _pos; }

private:
  bool collideWithWorld(const std::vector<std::string> &levelData);

  float _dmg;
  float _vel;
  glm::vec2 _pos;
  glm::vec2 _dir;
};
