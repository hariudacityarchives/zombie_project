#include "Bullet.h"

#include "../JEngine/ResourceManager.h"

#include "Human.h"
#include "Level.h"
#include "Zombie.h"

const unsigned char BULLET_RADIUS = 5;

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float vel, float dmg)
    : _pos(pos), _dir(dir), _vel(vel), _dmg(dmg) {}

Bullet::~Bullet() {}

bool Bullet::update(const std::vector<std::string> &levelData,
                    float deltaTime) {
  _pos += _dir * _vel * deltaTime;

  return collideWithWorld(levelData);
}

void Bullet::draw(JEngine::SpriteBatch &sb) {
  glm::vec4 destRect(_pos.x + BULLET_RADIUS, _pos.y + BULLET_RADIUS,
                     BULLET_RADIUS * 2, BULLET_RADIUS * 2);
  glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);

  JEngine::ColorRGBA8 color;
  color.setColor(75, 75, 75);

  sb.draw(destRect, uv,
          JEngine::ResourceManager::getTexture(
              "../graphicsStuff/Textures/circle.png")
              .id,
          0.0f, color);
}

bool Bullet::collideWithAgent(Agent *agent) {
  // I minimized a little... :P
  return (
      (AGENT_RADIUS + BULLET_RADIUS) -
          glm::length(_pos - (agent->getPosition() + glm::vec2(AGENT_RADIUS))) >
      0);
}

bool Bullet::collideWithWorld(const std::vector<std::string> &levelData) {
  glm::ivec2 gridPosition;
  gridPosition.x = floor(_pos.x / (float)TILE_WIDTH);
  gridPosition.y = floor(_pos.y / (float)TILE_WIDTH);

  // check if out of bounds
  if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length() ||
      gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
    return true;
  }

  return levelData[gridPosition.y][gridPosition.x] != '.';
}