#include "Potion.h"
#include "Level.h"
#include "Player.h"

#include "../JEngine/ResourceManager.h"

#include <random>
#include <time.h>

Potion::Potion(const std::vector<std::string> &levelData) {
  // changed to static to prevent duplicates(it really happens)
  static std::mt19937 ran = std::mt19937(time(nullptr));
  static std::uniform_int_distribution<int> ranX(1, levelData[0].size() - 1);
  static std::uniform_int_distribution<int> ranY(1, levelData.size() - 1);
  static std::uniform_real_distribution<float> ranTick(6000.0f, 11000.0f);
  static std::uniform_int_distribution<int> ranColor(0, 255);
  static std::uniform_int_distribution<int> ranEffect(
      0, (int)PotionEffect::END - 1);
  static std::uniform_int_distribution<int> ranAdditionalPos(0, POTION_SIZE);

  // a little inefficient and unsafe.
  int x = 0;
  int y = 0;
  do {
    x = ranX(ran);
    y = ranY(ran);
  } while (levelData[y][x] != '.');

  _pos = glm::vec2(x * TILE_WIDTH + ranAdditionalPos(ran),
                   y * TILE_WIDTH + ranAdditionalPos(ran));
  _color.setColor(ranColor(ran), ranColor(ran), ranColor(ran));
  _effect = (PotionEffect)ranEffect(ran);
  _timeToLive = ranTick(ran) / DESIRED_FRAMETIME;
}

Potion::~Potion() {}

bool Potion::collideWithPlayer(Player *player) {
  return ((AGENT_RADIUS + POTION_RADIUS) -
              glm::length(_pos -
                          (player->getPosition() + glm::vec2(AGENT_RADIUS))) >
          0);
}

bool Potion::update(float deltaTime) {
  _timeToLive -= deltaTime;
  return _timeToLive <= 0;
}

void Potion::draw(JEngine::SpriteBatch &sb) {
  static int textureID =
      JEngine::ResourceManager::getTexture("Textures/potion.png").id;

  const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

  glm::vec4 destRect;
  destRect.x = _pos.x;
  destRect.y = _pos.y;
  destRect.z = POTION_SIZE;
  destRect.w = POTION_SIZE;

  sb.draw(destRect, uvRect, textureID, 0.0f, _color);
}
