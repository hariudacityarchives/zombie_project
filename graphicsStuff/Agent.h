#pragma once

#include "../JEngine/SpriteBatch.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

const unsigned short AGENT_WIDTH = 60;
const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent {
public:
  Agent();
  virtual ~Agent();

  void draw(JEngine::SpriteBatch &sb);

  virtual void update(const std::vector<std::string> &levelData,
                      std::vector<Human *> &humans,
                      std::vector<Zombie *> &zombies, float deltaTime) = 0;

  bool collideWithLevel(const std::vector<std::string> &levelData);

  bool collideWithAgent(Agent *agent);

  // returns if dieded
  bool applyDamage(float damage);

  glm::vec2 getPosition() const { return _pos; }

protected:
  void checkTileCollision(std::vector<glm::vec2> &cTiles,
                          const std::vector<std::string> &data, float x,
                          float y);
  void collideWithTile(glm::vec2 tilePos);

  glm::vec2 _pos;
  glm::vec2 _direction; // = glm::vec2(1.0f, 0.0f);
  JEngine::ColorRGBA8 _color;
  float _speed;
  float _health;
  GLuint _textureID;
};
