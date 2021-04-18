#pragma once

#include "Bullet.h"

#include "../JEngine/AudioEngine.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Gun {
public:
  Gun(std::string name, int fireRate, int bulletsPerShot, float spread,
      float bulletDamage, float bulletSpeed);
  ~Gun();

  void amplifyDamage();
  void amplifyRate();

  void updatePotions(float deltaTime);
  void update(bool isMouseDown, const glm::vec2 &position, glm::vec2 direction,
              std::vector<Bullet> &bullets, float deltaTime);

  std::string getName() const { return _name; }

private:
  void fire(const glm::vec2 &position, const glm::vec2 &direction,
            std::vector<Bullet> &bullets);

  std::string _name;

  JEngine::SoundEffect _fireSfx;

  int _fireRate;       // fire rate in terms of frames
  int _bulletsPerShot; // How many bullets are fired at the same time
  float _spread;       // accuracy
  int _bulletSpeed;    // speed of the bullet
  float _bulletDamage; // damage of the bullet
  float _frameCounter;

  float _damageTime;
  float _rateTime;
};
