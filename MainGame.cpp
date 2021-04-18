#include "MainGame.h"

#include "graphicsStuff/Constants.h"
#include "graphicsStuff/Gun.h"
#include "graphicsStuff/Zombie.h"

#include "JEngine/Errors.h"
#include "JEngine/JEngine.h"
#include "JEngine/ResourceManager.h"
#include "JEngine/Timing.h"

#include <glm/gtx/rotate_vector.hpp>

#include <SDL2/SDL.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>

namespace sys = JEngine; // when lazyness strikes and you can use it

const float HUMAN_SPEED = 0.9804f;
const float ZOMBIE_SPEED = 1.3f;

const unsigned char NUM_POTIONS = 8;

// TODO: Fix potions

MainGame::MainGame()
    : _gameState(GameState::PLAY), _fps(0), _currentLevel(0), _player(nullptr),
      _humansBecomeZombie(0), _humansKilled(0), _zombiesKilled(0) {
  // Empty
}

MainGame::~MainGame() {
  for (int i = 0; i < _levels.size(); i++) {
    delete _levels[i];
  }

  for (int i = 0; i < _humans.size(); i++) {
    delete _humans[i];
  }

  for (int i = 0; i < _zombies.size(); i++) {
    delete _zombies[i];
  }
}

void MainGame::run() {
  initSystems();

  // sys::Music mus = _audioEngine.loadMusic("Sound/Leon Bolier - The Wolf
  // (Original Mix).mp3"); mus.play(-1);

  gameLoop();
}

void MainGame::initSystems() {
  sys::init();

  _window.create("Zombie game!", _screenWidth, _screenHeight, 0);
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

  initShaders();

  _camera.init(_screenWidth, _screenHeight);
  _hudCamera.init(_screenWidth, _screenHeight);

  _agentSpriteBatch.init();
  _hudSpriteBatch.init();
  _hudCamera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));

  _spriteFont = new sys::SpriteFont("../graphicsStuff/Fonts/hacked.ttf", 64);

  _broadcaster.init(_screenWidth, _screenHeight, _spriteFont->getFontHeight(),
                    0.45f);

  _audioEngine = sys::AudioEngine();
  _audioEngine.init();

  initLevel();

  // the [] accepts arguments:
  // = : pass em in as a copy
  // & : pass em in as a reference
  // http://www.cprogramming.com/c++11/c++11-lambda-closures.html

  // init particles
  _bloodParticleBatch = new sys::ParticleBatch2D;
  _bloodParticleBatch->init(
      1000, 0.0216f, sys::ResourceManager::getTexture("Textures/particle.png"),
      [](JEngine::Particle2D &p, float deltaTime) {
        p.pos += p.vel * deltaTime;
        p.color.a = (GLubyte)(p.lifeTime * 255.0f);
      });
  _particleEngine.addParticleBatch(_bloodParticleBatch);
}

void MainGame::initLevel() {
  // add level 1
  _levels.push_back(new Level("./graphicsStuff/Levels/level1.txt"));
  _currentLevel = 0;

  _bullets = std::vector<Bullet>();

  _player = new Player();
  _player->init(4, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager,
                &_camera, &_bullets);

  _humans.push_back(_player);

  std::mt19937 randomEngine;
  randomEngine.seed(time(nullptr));
  std::uniform_int_distribution<int> ranX(
      2, _levels[_currentLevel]->getWidth() - 2);
  std::uniform_int_distribution<int> ranY(
      2, _levels[_currentLevel]->getHeight() - 2);

  const int TILE_SIZE = 64;

  for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
    _humans.push_back(new Human);
    _humans.back()->init(HUMAN_SPEED,
                         glm::vec2(ranX(randomEngine) * TILE_SIZE,
                                   ranY(randomEngine) * TILE_SIZE));
  }

  // Add the zombies
  std::vector<glm::ivec2> zombiePositions =
      _levels[_currentLevel]->getZombieStartPositions();
  for (int i = 0; i < zombiePositions.size(); i++) {
    _zombies.push_back(new Zombie);
    _zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
  }

  // Add the potions
  for (int i = 0; i < NUM_POTIONS; i++) {
    _potions.emplace_back(_levels[_currentLevel]->getLevelData());
  }

  // Set up guns
  const float BULLET_SPEED = 11.0f;
  _player->addGun(
      new Gun("Magnum", 22, 1, 5.0f, 30, BULLET_SPEED,
              _audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
  _player->addGun(
      new Gun("Shotgun", 40, 12, 20.0f, 7, BULLET_SPEED,
              _audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
  _player->addGun(new Gun("MP5", 8, 1, 11.0f, 20, BULLET_SPEED,
                          _audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
  _player->addGun(new Gun("M16", 23, 3, 5.1f, 10, BULLET_SPEED,
                          _audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
}

void MainGame::initShaders() {
  // Compile our color shader
  _textureProgram.compileShaders(
      "../graphicsStuff/Shaders/textureShading.vert",
      "../graphicsStuff/Shaders/textureShading.frag");
  _textureProgram.addAttribute("vertexPosition");
  _textureProgram.addAttribute("vertexColor");
  _textureProgram.addAttribute("vertexUV");
  _textureProgram.linkShaders();
}

void MainGame::gameLoop() {

  sys::FpsLimiter fpsLimiter;
  fpsLimiter.setTargetFPS(WANTED_FPS);

  const float MAX_TICKS = 1.0f;
  const unsigned short MAX_CALCS = 5;

  Uint32 prevTicks = SDL_GetTicks();

  while (_gameState == GameState::PLAY) {
    fpsLimiter.begin();

    Uint32 newTicks = SDL_GetTicks();
    float frameTime = newTicks - prevTicks; // actual deltatime in milliseconds
    prevTicks = newTicks;

    float totalDeltaTime =
        frameTime / DESIRED_FRAMETIME; // how many frames it'll take

    checkVictory();
    _inputManager.update();
    processInput();

    int i = 0;
    while (totalDeltaTime > 0.0f && i < MAX_CALCS) {
      float deltaTime = std::min(totalDeltaTime, MAX_TICKS);

      updateAgents(deltaTime);
      updateBullets(deltaTime);
      updatePotions(deltaTime);
      _particleEngine.update(deltaTime);
      _broadcaster.update(deltaTime);

      totalDeltaTime -= deltaTime;
      i++;
    }

    _camera.setPosition(_player->getPosition());
    _camera.update();

    _hudCamera.update();

    drawGame();

    _fps = fpsLimiter.end();
  }
}

void MainGame::checkVictory() {
  if (_zombies.empty()) {
    std::cout << "\nYou win!\nZombies killed: " << _zombiesKilled
              << "\nHumans killed: " << _humansKilled
              << "\nHumans remaining: " << _humans.size()
              << "\nHumans become zombie: " << _humansBecomeZombie << std::endl;
    JEngine::fatalError("runtimerror fake derp I just want to quit mkay");
  }
}

void MainGame::updateAgents(float deltaTime) {
  // update humans
  for (int i = 0; i < _humans.size(); i++) {
    _humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans,
                       _zombies, deltaTime);
  }

  // update zombies
  for (int i = 0; i < _zombies.size(); i++) {
    _zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans,
                        _zombies, deltaTime);
  }

  // collisions humans
  for (int i = 0; i < _humans.size(); i++) {
    for (int j = i + 1; j < _humans.size(); j++) {
      _humans[i]->collideWithAgent(_humans[j]);
    }
  }

  // collisions zombies
  for (int i = 0; i < _zombies.size(); i++) {
    for (int j = i + 1; j < _zombies.size(); j++) {
      _zombies[i]->collideWithAgent(_zombies[j]);
    }

    for (int j = 1; j < _humans.size(); j++) {
      if (_zombies[i]->collideWithAgent(_humans[j])) {
        // add new zombie on collision
        _zombies.push_back(new Zombie);
        _zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());

        delete _humans[j];
        _humans[j] = _humans.back();
        _humans.pop_back();

        _humansBecomeZombie++;
      }
    }

    // check collision with player
    if (_zombies[i]->collideWithAgent(_player) && _player->isVulnerable()) {
      sys::fatalError("Ha LOSER!!!!111");
    }
  }
}

void MainGame::updateBullets(float deltaTime) {
  // update & collide with world
  for (int i = 0; i < _bullets.size();) {
    if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
      _bullets[i] = _bullets.back();
      _bullets.pop_back();
    } else {
      i++;
    }
  }

  // Collide with humans
  for (int i = 0; i < _bullets.size(); i++) {
    for (int j = 0; j < _zombies.size(); j++) {
      if (_bullets[i].collideWithAgent(_zombies[j])) {
        addBloodParticle(_bullets[i].getPosition(), 5);

        // damage, and kill if out of health
        if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
          delete _zombies[j];
          _zombies[j] = _zombies.back();
          _zombies.pop_back();

          _zombiesKilled++;
        }

        _bullets[i] = _bullets.back();
        _bullets.pop_back();

        i--;

        break;
      }
    }
  }

  for (int i = 0; i < _bullets.size(); i++) {
    // player is invincible for bullets
    for (int j = 1; j < _humans.size(); j++) {
      if (_bullets[i].collideWithAgent(_humans[j])) {
        // add blood
        addBloodParticle(_bullets[i].getPosition(), 5);

        // damage, and kill if out of health
        if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
          delete _humans[j];
          _humans[j] = _humans.back();
          _humans.pop_back();

          _humansKilled++;
        }

        _bullets[i] = _bullets.back();
        _bullets.pop_back();

        i--;

        break;
      }
    }
  }
}

void MainGame::updatePotions(float deltaTime) {
  for (int i = 0; i < NUM_POTIONS; i++) {
    if (_potions[i].update(deltaTime)) {
      // doing this line twice is effecienter than a bool or something.
      _potions[i] = Potion(_levels[_currentLevel]->getLevelData());
    } else if (_potions[i].collideWithPlayer(_player)) {
      _player->addPotion(_potions[i].getPotionEffect(), &_broadcaster);
      _potions[i] = Potion(_levels[_currentLevel]->getLevelData());
    }
  }
}

void MainGame::processInput() {
  SDL_Event evnt;
  // Will keep looping until there are no more events to process
  while (SDL_PollEvent(&evnt)) {
    switch (evnt.type) {
    case SDL_QUIT:
      // Exit the game here!
      break;
    case SDL_MOUSEMOTION:
      _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
      break;
    case SDL_KEYDOWN:
      _inputManager.pressKey(evnt.key.keysym.sym);
      break;
    case SDL_KEYUP:
      _inputManager.releaseKey(evnt.key.keysym.sym);
      break;
    case SDL_MOUSEBUTTONDOWN:
      _inputManager.pressKey(evnt.button.button);
      break;
    case SDL_MOUSEBUTTONUP:
      _inputManager.releaseKey(evnt.button.button);
      break;
    }
  }

  if (_inputManager.isKeyDown(SDLK_ESCAPE)) {
    _gameState = GameState::EXIT;
  }

  const float CAMERA_SPEED = 0.01f;
  if (_inputManager.isKeyDown(SDLK_q)) {
    _camera.setScale(_camera.getScale() + CAMERA_SPEED);
  } else if (_inputManager.isKeyDown(SDLK_e)) {
    _camera.setScale(_camera.getScale() - CAMERA_SPEED);
  }
}

void MainGame::drawGame() {
  // Set the base depth to 1.0
  glClearDepth(1.0);
  // Clear the color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _textureProgram.use();

  glActiveTexture(GL_TEXTURE0);

  // make sure the shader uses texture 0
  GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
  glUniform1i(textureUniform, 0);

  glm::mat4 projectionMatrix = _camera.getCameraMatrix();
  GLint pUniform = _textureProgram.getUniformLocation("P");
  glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

  // draw level
  _levels[_currentLevel]->draw();

  // draw agents
  _agentSpriteBatch.begin();

  // draw the agents

  const glm::vec2 agentDim(AGENT_WIDTH);

  for (int i = 0; i < NUM_POTIONS; i++) {
    _potions[i].draw(_agentSpriteBatch);
  }

  for (int i = 0; i < _humans.size(); i++) {
    if (_camera.isBoxInView(_humans[i]->getPosition(), agentDim)) {
      _humans[i]->draw(_agentSpriteBatch);
    }
  }

  for (int i = 0; i < _zombies.size(); i++) {
    if (_camera.isBoxInView(_zombies[i]->getPosition(), agentDim)) {
      _zombies[i]->draw(_agentSpriteBatch);
    }
  }

  // not just for agents LOL
  for (int i = 0; i < _bullets.size(); i++) {
    _bullets[i].draw(_agentSpriteBatch);
  }

  _agentSpriteBatch.end();
  _agentSpriteBatch.renderBatch();

  _particleEngine.draw(&_agentSpriteBatch);

  drawHUD();

  _textureProgram.unuse();

  // Swap our buffer and draw everything to the screen!
  _window.swapBuffer();
}

void MainGame::drawHUD() {
  char buffer[256];

  glm::mat4 projectionMatrix = _hudCamera.getCameraMatrix();
  GLint pUniform = _textureProgram.getUniformLocation("P");
  glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

  _hudSpriteBatch.begin();

  glm::vec2 pos(10, _screenHeight - 74);

  snprintf(buffer, 100, "Humans : %d", _humans.size());

  _spriteFont->draw(_hudSpriteBatch, buffer, pos, glm::vec2(1.0f), 0.0f,
                    sys::ColorRGBA8(255, 255, 255, 255));

  pos.y -= 64;
  snprintf(buffer, 100, "Zombies: %d", _zombies.size());

  _spriteFont->draw(_hudSpriteBatch, buffer, pos, glm::vec2(1.0f), 0.0f,
                    sys::ColorRGBA8(255, 255, 255, 255));

  // broadcasted message
  _broadcaster.draw(_spriteFont, _hudSpriteBatch);

  _hudSpriteBatch.end();
  _hudSpriteBatch.renderBatch();
}

void MainGame::addBloodParticle(const glm::vec2 &pos, int numParticles) {
  static std::mt19937 ranEngine(time(nullptr));
  static std::uniform_real_distribution<float> ranAngle(0.0f, 360.0f);
  static std::uniform_real_distribution<float> ranNum(20.12f, 50.12f);

  glm::vec2 vel(1.12f, 0.0f);
  sys::ColorRGBA8 clr(255, 0, 0, 255);

  for (int i = 0; i < numParticles; i++) {
    _bloodParticleBatch->addParticle(pos, glm::rotate(vel, ranAngle(ranEngine)),
                                     clr, ranNum(ranEngine));
  }
}