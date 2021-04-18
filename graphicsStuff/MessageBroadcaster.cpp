#include "MessageBroadcaster.h"

#include <string>

MessageBroadcaster::MessageBroadcaster() {}

MessageBroadcaster::~MessageBroadcaster() {}

void MessageBroadcaster::init(int screenWidth, int screenHeight, int fontHeight,
                              float scale) {
  _scale = scale;
  _pos = glm::vec2(screenWidth / 2, screenHeight / 2 + fontHeight * scale / 2);
}

void MessageBroadcaster::broadcast(std::string msg, float reqTime) {
  _col = JEngine::ColorRGBA8(255, 255, 255);

  _message = msg;
  _timeLeft = reqTime;
  _startingTime = reqTime;
}

void MessageBroadcaster::update(float deltaTime) {
  if (_timeLeft > 0.0f) {
    _timeLeft = _timeLeft - deltaTime / 2.5f -
                ((_startingTime - _timeLeft) * (_startingTime - _timeLeft) *
                 deltaTime * 0.00043f);
    _col.a = (GLubyte)((_timeLeft / _startingTime) * 255.0f);
  }
}

void MessageBroadcaster::draw(JEngine::SpriteFont *sf,
                              JEngine::SpriteBatch &sb) {
  if (_timeLeft > 0.0f) {
    char *buffer;
    buffer = &_message[0];
    sf->draw(sb, buffer, _pos, glm::vec2(_scale), 0.0f, _col,
             JEngine::Justification::MIDDLE);
  }
}