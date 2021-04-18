#include "Player.h"

#include "Gun.h"
#include "MessageBroadcaster.h"
#include "Potion.h"

#include "../JEngine/InputManager.h"
#include "../JEngine/ResourceManager.h"

#include <SDL2/SDL.h>

//#include <iostream>

Player::Player() : _currentGun(-1), _isInvincible(false) {}

Player::~Player() {}

void Player::addPotion(PotionEffect effect, MessageBroadcaster *broadcaster) {
  switch (effect) {
  case PotionEffect::SPEED_UP:
    if (_speedUpTime <= 0.0f)
      _speed *= 2;
    _speedUpTime = POTION_TIME;
    broadcaster->broadcast("Picked up a speed potion!");
    break;
  case PotionEffect::INVINCIBLE:
    if (_invincibleTime <= 0.0f)
      _isInvincible = true;
    _invincibleTime = POTION_TIME / 2.0f;
    broadcaster->broadcast("You are now immortal for a few seconds!");
    break;
  case PotionEffect::EXTRA_DAMAGE:
    _guns[_currentGun]->amplifyDamage();
    broadcaster->broadcast("Picked up a damage potion for your " +
                           _guns[_currentGun]->getName() + "!");
    break;
  case PotionEffect::FASTER_SHOOTING:
    _guns[_currentGun]->amplifyRate();
    broadcaster->broadcast("Picked up a firerate potion for your " +
                           _guns[_currentGun]->getName() + "!");
    break;
  }
}

void Player::addGun(Gun *gun) {
  _guns.push_back(gun);

  if (_currentGun == -1) {
    _currentGun = 0;
  }
}

void Player::init(float speed, glm::vec2 pos,
                  JEngine::InputManager *inputManager,
                  JEngine::Camera2D *camera, std::vector<Bullet> *bullets) {
  _speed = speed;
  _pos = pos;
  _color.setColor(255, 255, 255);
  _inputManager = inputManager;
  _camera = camera;
  _bullets = bullets;
  _health = 100.0f;
  _textureID = JEngine::ResourceManager::getTexture("Textures/player.png").id;
}

void Player::update(const std::vector<std::string> &levelData,
                    std::vector<Human *> &humans,
                    std::vector<Zombie *> &zombies, float deltaTime) {
  if (_speedUpTime > 0.0f) {
    _speedUpTime -= deltaTime;
    if (_speedUpTime <= 0.0f) {
      _speed /= 2;
    }
  }

  if (_invincibleTime > 0.0f) {
    _invincibleTime -= deltaTime;
    if (_invincibleTime <= 0.0f) {
      _isInvincible = false;
    }
  }

  if (_inputManager->isKeyDown(SDLK_d)) {
    _pos.x += _speed * deltaTime;
  } else if (_inputManager->isKeyDown(SDLK_a)) {
    _pos.x -= _speed * deltaTime;
  }

  if (_inputManager->isKeyDown(SDLK_w)) {
    _pos.y += _speed * deltaTime;
  } else if (_inputManager->isKeyDown(SDLK_s)) {
    _pos.y -= _speed * deltaTime;
  }

  if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0) {
    _currentGun = 0;
  }
  if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1) {
    _currentGun = 1;
  }
  if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2) {
    _currentGun = 2;
  }
  if (_inputManager->isKeyDown(SDLK_4) && _guns.size() >= 3) {
    _currentGun = 3;
  }

  glm::vec2 mouseCoords = _inputManager->getMouseCoords();
  mouseCoords = _camera->convertScreenToWorld(mouseCoords);

  glm::vec2 centerPos = _pos + glm::vec2(AGENT_RADIUS);

  _direction = glm::normalize(mouseCoords - centerPos);

  // TODO: FIX POTIONs
  _guns[_currentGun]->updatePotions(deltaTime);

  if (_currentGun != -1) {
    _guns[_currentGun]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
                               centerPos, _direction, *_bullets, deltaTime);
  }

  collideWithLevel(levelData);
}