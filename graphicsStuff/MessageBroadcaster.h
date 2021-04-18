#pragma once

#include "Constants.h"

#include <glm/glm.hpp>

#include "../JEngine/SpriteBatch.h"
#include "../JEngine/SpriteFont.h"

const float DEFAULT_MESSAGE_BROADCAST_TIME = 2500.0f / DESIRED_FRAMETIME;

class MessageBroadcaster {
public:
  MessageBroadcaster();
  ~MessageBroadcaster();

  void init(int screenWidth, int screenHeight, int fontHeight, float scale);

  void broadcast(std::string msg,
                 float reqTime = DEFAULT_MESSAGE_BROADCAST_TIME);
  void update(float deltaTime);
  void draw(JEngine::SpriteFont *sf, JEngine::SpriteBatch &sb);

  // getters
  bool isDisplaying() const { return _timeLeft > 0.0f; }

private:
  float _scale = 1.0f;
  float _timeLeft = 0;
  float _startingTime = 0;
  std::string _message;

  glm::vec2 _pos;
  JEngine::ColorRGBA8 _col;
};
