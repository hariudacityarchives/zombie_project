#include "Timing.h"

#include <SDL2/SDL.h>
#include <iostream>

namespace JEngine {
FpsLimiter::FpsLimiter() // : _timeElapsed(0.0f)
{}

void FpsLimiter::init(float targetFps) {
  // for now, you see
  setTargetFPS(targetFps);
}

void FpsLimiter::setTargetFPS(float targetFps) { _targetFps = targetFps; }

void FpsLimiter::begin() {}

// will return FPS
float FpsLimiter::end() {
  _startTicks = SDL_GetTicks();

  static const int NUM_SAMPLES = 10;
  static float frameTimes[NUM_SAMPLES];
  static int currentFrame = 0;

  static float prevTicks = SDL_GetTicks();

  float currentTicks = SDL_GetTicks();

  _frameTime = currentTicks - prevTicks;
  frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

  prevTicks = currentTicks;

  int count;
  currentFrame++;

  if (currentFrame < NUM_SAMPLES) {
    count = currentFrame;
  } else {
    count = NUM_SAMPLES;
  }

  float frameTimeAvg = 0;
  for (int i = 0; i < count; i++) {
    frameTimeAvg += frameTimes[i];
  }
  frameTimeAvg /= count;

  if (frameTimeAvg > 0) {
    _fps = 1000.0f / frameTimeAvg;
  } else {
    _fps = -1; // FAKE ITTTTTTTTT
  }

  // limit
  float frameTicks = SDL_GetTicks() - _startTicks;
  //_timeElapsed += _frameTime;

  // limit FPS
  float wantedTicks = 1000.0f / _targetFps;
  if (wantedTicks > frameTicks) {
    SDL_Delay(wantedTicks - frameTicks);
  }

  // PRINT OUT EVERY SECOND
  /////////////////////////
  /*if (_timeElapsed >= 1000.0f)
  {
          std::cout << _fps << std::endl;
          _timeElapsed = 0.0f;
  }*/

  // return
  return _fps;
}
} // namespace JEngine