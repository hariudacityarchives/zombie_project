#include "Window.h"
#include "Errors.h"

namespace JEngine {
Window::Window() {}

Window::~Window() {}

// doesn't return actual errors... xD
int Window::create(std::string windowName, int screenWidth, int screenHeight,
                   unsigned int currentFlags) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;

  Uint32 flags = SDL_WINDOW_OPENGL;

  if (currentFlags & INVISIBLE) {
    flags |= SDL_WINDOW_HIDDEN;
  }
  if (currentFlags & BORDERLESS) {
    flags |= SDL_WINDOW_BORDERLESS;
  }
  if (currentFlags & FULLSCREEN) {
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }

  // old title: "SUPER AWESOME C++ SDL OPENGL HIGH-PERFORMANCE LOW HARDWARE
  // GAME"
  _window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight,
                             flags);

  // holy! We've got a null window!
  if (_window == nullptr) {
    fatalError("NullPointerException: SDL Window could not be opened created!");
  }

  SDL_GLContext glContext = SDL_GL_CreateContext(_window);
  if (glContext == nullptr) {
    fatalError("NullPointerException: SDL_GL context could not be created!");
  }

  GLenum error = glewInit();
  if (error != GLEW_OK) // not okay, gimme a error!
  {
    std::string msg = "Could not initialize GLEW: ";

    // did this for fun :3
    switch (error) {
    case GLEW_ERROR_NO_GL_VERSION:
      msg += "No OpenGL detected!";
      break;
    case GLEW_ERROR_GL_VERSION_10_ONLY:
      msg += "At least OpenGL 1.1 is required!";
      break;
    case GLEW_ERROR_GLX_VERSION_11_ONLY:
      msg += "At least GLX 1.2 is required!";
      break;
    default:
      msg += "Unkown fatal error(@ GLEW_INIT)! Call the police! Code: " + error;
      break;
    }

    std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    fatalError(msg);
  }

  // print out GL version
  std::printf("***   OpenGL Version: %s   ***\n", glGetString(GL_VERSION));

  // set the color you want to clear every draw!
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

  // V-Sync
  if (currentFlags & VSYNC) {
    SDL_GL_SetSwapInterval(1);
  } else {
    SDL_GL_SetSwapInterval(0);
  }

  // enable alpha blend
  glEnable(GL_BLEND);

  // multiply alpha!
  // args: how to blend, what portion of color that's already on the screen do I
  // use
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return 0;
}

void Window::swapBuffer() { SDL_GL_SwapWindow(_window); }
} // namespace JEngine