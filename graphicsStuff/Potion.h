#pragma once

#include "Constants.h"

#include "../JEngine/SpriteBatch.h"
#include <glm/glm.hpp>
#include <string>

enum class PotionEffect {
  SPEED_UP,
  EXTRA_DAMAGE,
  FASTER_SHOOTING,
  INVINCIBLE,
  END
};

const unsigned short POTION_SIZE = 32;
const unsigned short POTION_RADIUS = 16;

const float POTION_TIME = 15000.0f / DESIRED_FRAMETIME;

class Player;

class Potion {
public:
  Potion(const std::vector<std::string> &levelData);
  ~Potion();

  void draw(JEngine::SpriteBatch &sb);

  bool collideWithPlayer(Player *player);

  bool update(float deltaTime);

  // getters
  PotionEffect getPotionEffect() const { return _effect; }

private:
  JEngine::ColorRGBA8 _color;
  glm::vec2 _pos;
  PotionEffect _effect;

  unsigned int _timeToLive;
};
