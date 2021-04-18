#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "JEngine.h"

namespace JEngine {
// this is boring
int init() {
  // init SDL
  SDL_Init(SDL_INIT_EVERYTHING);

  // we want to doublebuffer
  // draw on one window while the other is being cleared
  // no flicker!
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  return 0;
}
} // namespace JEngine