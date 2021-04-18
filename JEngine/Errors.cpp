#include "Errors.h"

#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>

namespace JEngine {
void fatalError(std::string errorString) {
  std::cout << "Fatal error(probably due to a LOLCAT):" << std::endl;
  std::cout << "0x" << &errorString << ": " << errorString << std::endl;
  std::cout << "\nPress the enter key to quit...";
  int a;
  std::cin >> a;
  SDL_Quit();
  exit(69);
}
} // namespace JEngine