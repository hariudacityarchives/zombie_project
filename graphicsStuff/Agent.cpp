#include "Agent.h"
#include "Level.h"

#include "../JEngine/ResourceManager.h"
#include <algorithm>

const unsigned char TILE_SIZE = 64;

Agent::Agent() {}

Agent::~Agent() {}

bool Agent::collideWithLevel(const std::vector<std::string> &levelData) {
  std::vector<glm::vec2> cTilePoss;

  // check the 4 corners
  checkTileCollision(cTilePoss, levelData, _pos.x, _pos.y);
  checkTileCollision(cTilePoss, levelData, _pos.x + AGENT_WIDTH, _pos.y);
  checkTileCollision(cTilePoss, levelData, _pos.x, _pos.y + AGENT_WIDTH);
  checkTileCollision(cTilePoss, levelData, _pos.x + AGENT_WIDTH,
                     _pos.y + AGENT_WIDTH);

  // no blocks collided
  if (cTilePoss.size() == 0) {
    return false;
  }

  for (int i = 0; i < cTilePoss.size(); i++) {
    // do the collision
    collideWithTile(cTilePoss[i]);
  }

  return true;
}

bool Agent::collideWithAgent(Agent *agent) {
  glm::vec2 centerPosA = _pos + glm::vec2(AGENT_RADIUS);
  glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

  glm::vec2 distVec = centerPosA - centerPosB;

  float distance = glm::length(distVec);
  float depth = AGENT_WIDTH - distance;

  if (depth > 0) {
    glm::vec2 collisionDepthVec = (glm::normalize(distVec) * depth) / 2.0f;

    _pos += collisionDepthVec;
    agent->_pos -= collisionDepthVec;
    return true;
  }
  return false;
}

void Agent::checkTileCollision(std::vector<glm::vec2> &cTiles,
                               const std::vector<std::string> &data, float x,
                               float y) {
  glm::vec2 cornerPos =
      glm::vec2(floor(x / (float)TILE_SIZE), floor(y / (float)TILE_SIZE));

  // if out of bounds return
  /*if (cornerPos.x < 0 || cornerPos.x >= data[0].length() ||
          cornerPos.y < 0 || cornerPos.y >= data.size())
  {
          return;
  }*/

  if (data[cornerPos.y][cornerPos.x] != '.') {
    cTiles.push_back(cornerPos * (float)TILE_SIZE +
                     glm::vec2(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f));
  }
}

bool Agent::applyDamage(float damage) {
  _health -= damage;
  return (_health <= 0);
}

// AABB COLLISION!
void Agent::collideWithTile(glm::vec2 tilePos) {
  // const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;
  const float TILE_RADIUS = (float)TILE_SIZE / 2.0f;
  const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

  glm::vec2 centerPlayerPos = _pos + glm::vec2(AGENT_RADIUS);
  glm::vec2 distVec = centerPlayerPos - tilePos;

  float xDepth = MIN_DISTANCE - abs(distVec.x);
  float yDepth = MIN_DISTANCE - abs(distVec.y);

  // LOL will always be true
  if (std::max(xDepth, 0.0f) || std::max(yDepth, 0.0f)) {
    if (xDepth < yDepth) {
      if (distVec.x < 0) {
        _pos.x -= xDepth;
      } else {
        _pos.x += xDepth;
      }
    } else {
      if (distVec.y < 0) {
        _pos.y -= yDepth;
      } else {
        _pos.y += yDepth;
      }
    }
  }
}

void Agent::draw(JEngine::SpriteBatch &sb) {
  const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

  glm::vec4 destRect;
  destRect.x = _pos.x;
  destRect.y = _pos.y;
  destRect.z = AGENT_WIDTH;
  destRect.w = AGENT_WIDTH;

  sb.draw(destRect, uvRect, _textureID, 0.0f, _color, _direction);
}
