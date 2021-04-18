#include "Gun.h"
#include "Potion.h"

#include <glm/gtx/rotate_vector.hpp>
#include <random>
#include <time.h>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread,
         float bulletDamage, float bulletSpeed)
    : _name(name), _fireRate(fireRate), _bulletsPerShot(bulletsPerShot),
      _spread(spread), _bulletDamage(bulletDamage), _bulletSpeed(bulletSpeed),
      _frameCounter(0), _rateTime(0.0f), _damageTime(0.0f) {}

Gun::~Gun() {}

// well shit.
// forgot I had multiple guns.
void Gun::amplifyDamage() {
  if (_damageTime <= 0.0f)
    _bulletDamage *= 4;
  _damageTime = POTION_TIME;
}

void Gun::amplifyRate() {
  if (_rateTime <= 0.0f)
    _fireRate /= 2;
  _rateTime = POTION_TIME;
}

void Gun::updatePotions(float deltaTime) {
  if (_damageTime > 0.0f) {
    _damageTime -= deltaTime;
    if (_damageTime <= 0.0f) {
      _bulletDamage /= 4;
    }
  }

  if (_rateTime > 0.0f) {
    _rateTime -= deltaTime;
    if (_rateTime <= 0.0f) {
      _rateTime *= 2;
    }
  }
}

void Gun::update(bool isMouseDown, const glm::vec2 &position,
                 glm::vec2 direction, std::vector<Bullet> &bullets,
                 float deltaTime) {
  _frameCounter += 1.0f * deltaTime;
  if (_frameCounter >= _fireRate && isMouseDown) {
    fire(position, direction, bullets);
    _frameCounter = 0;
  }
}

void Gun::fire(const glm::vec2 &position, const glm::vec2 &direction,
               std::vector<Bullet> &bullets) {
  std::mt19937 randomEngine(time(nullptr));
  std::uniform_real_distribution<float> randomRotate(-_spread, _spread);

  //_fireSfx.play();

  for (int i = 0; i < _bulletsPerShot; i++) {
    bullets.emplace_back(position,
                         glm::rotate(direction, randomRotate(randomEngine)),
                         _bulletSpeed, _bulletDamage);
  }
}
