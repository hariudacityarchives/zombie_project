#pragma once

namespace JEngine {
class FpsLimiter {
public:
  FpsLimiter();

  void init(float targetFps);
  void setTargetFPS(float targetFps);

  void begin();

  // will return FPS
  float end();

private:
  // float _timeElapsed;
  float _fps;
  float _frameTime;
  float _targetFps;
  unsigned int _startTicks;
};
} // namespace JEngine