#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

namespace JEngine {
enum WindowFlags {
  INVISIBLE = 0x1,
  FULLSCREEN = 0x2,
  BORDERLESS = 0x4,
  VSYNC = 0x8
};

class Window {
public:
  Window();
  ~Window();

  int create(std::string windowName, int screenWidth, int screenHeight,
             unsigned int currentFlags);

  void swapBuffer();

  int getScreenWidth() { return _screenWidth; }

  int getScreenHeight() { return _screenHeight; }

private:
  SDL_Window *_window;
  int _screenWidth, _screenHeight;
};
} // namespace JEngine
