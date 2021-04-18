#pragma once

#include "JEngine/AudioEngine.h"
#include "JEngine/Camera2D.h"
#include "JEngine/GLSLProgram.h"
#include "JEngine/InputManager.h"
#include "JEngine/ParticleBatch2D.h"
#include "JEngine/ParticleEngine2D.h"
#include "JEngine/SpriteBatch.h"
#include "JEngine/SpriteFont.h"
#include "JEngine/Window.h"

#include "graphicsStuff/Level.h"
#include "graphicsStuff/Player.h"
#include "graphicsStuff/Potion.h"

#include "graphicsStuff/MessageBroadcaster.h"

class Zombie;

namespace sys = JEngine;

enum class GameState { PLAY, EXIT };

// Oh no.
// I just heard that _ is bad practice.
// whyyyyy so many little things you don't know?!
// anyways, I'm keeping it this project.
// You have to be consistent.
class MainGame {
public:
  MainGame();
  ~MainGame();

  /// Runs the game
  void run();

private:
  /// Initializes the core systems
  void initSystems();

  /// Initializes the shaders
  void initShaders();

  /// Main game loop for the program
  void gameLoop();

  /// Handles input processing
  void processInput();

  /// Renders the game
  void drawGame();
  void drawHUD(); // ! needs to be drawn in drawGame, not in game loop!(due to
                  // textureprograms n stuff)

  void updateAgents(float deltaTime);
  void updateBullets(float deltaTime);
  void initLevel();
  void updatePotions(float deltaTime);
  void checkVictory();
  void addBloodParticle(const glm::vec2 &pos, int numParticles);

  /// Member Variables
  sys::Window _window;              ///< The game window
  sys::GLSLProgram _textureProgram; ///< The shader program
  sys::InputManager _inputManager;  ///< Handles input

  sys::Camera2D _camera;    // Main Camera
  sys::Camera2D _hudCamera; // Camera for hud

  std::vector<Level *> _levels; // vector of all levels
  std::vector<Human *> _humans;
  std::vector<Zombie *> _zombies;
  std::vector<Bullet> _bullets;
  std::vector<Potion> _potions;
  Player *_player;

  sys::SpriteBatch _agentSpriteBatch;
  sys::SpriteBatch _hudSpriteBatch;

  sys::SpriteFont *_spriteFont;
  // sys::AudioEngine _audioEngine;

  sys::ParticleEngine2D _particleEngine;
  sys::ParticleBatch2D *_bloodParticleBatch;

  int _screenWidth = 800;
  int _screenHeight = 600;
  int _fps, _currentLevel = 0;
  int _humansKilled, _zombiesKilled, _humansBecomeZombie;

  GameState _gameState;
  MessageBroadcaster _broadcaster;
};
