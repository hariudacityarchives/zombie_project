#pragma once

#include "Bullet.h"
#include "Human.h"

#include "../JEngine/Camera2D.h"
#include "../JEngine/InputManager.h"

enum class PotionEffect;
class Gun;
class MessageBroadcaster;

class Player : public Human {
public:
  Player();
  ~Player();

  void init(float speed, glm::vec2 pos, JEngine::InputManager *inputManager,
            JEngine::Camera2D *camera, std::vector<Bullet> *bullets);

  void addGun(Gun *gun);

  void addPotion(PotionEffect effect, MessageBroadcaster *broadcaster);

  virtual void update(const std::vector<std::string> &levelData,
                      std::vector<Human *> &humans,
                      std::vector<Zombie *> &zombies, float deltaTime);

  // Getters
  bool isVulnerable() const { return !_isInvincible; }

private:
  JEngine::InputManager *_inputManager;
  JEngine::Camera2D *_camera;

  std::vector<Bullet> *_bullets;
  std::vector<Gun *> _guns;
  int _currentGun;

  float _speedUpTime;
  float _invincibleTime;
  bool _isInvincible;
};
