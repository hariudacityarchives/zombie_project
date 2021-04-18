#pragma once

#include <map>
#include <string>

#include <SDL2/SDL_mixer.h>

namespace JEngine {

class SoundEffect {
public:
  // Only the AudioEngine class can touch my private variables
  friend class AudioEngine;

  /// Plays effect file
  /// @param loops: If loops -1, loop forever(until variable limit reached),
  /// otherwise play if loops + 1 times
  void play(int loops = 0);

private:
  Mix_Chunk *_chunk = nullptr;
};

class Music {
public:
  friend class AudioEngine;

  /// Plays music
  /// @param times: If times == -1, loop forever(until variable limit reached),
  /// otherwise play it times times
  void play(int times = 1);

  // global functions
  static void pause();
  static void stop();
  static void resume();

private:
  Mix_Music *_music = nullptr;
};

class AudioEngine {
public:
  AudioEngine();
  ~AudioEngine();

  void init(int flags = MIX_INIT_MP3 | MIX_INIT_OGG);
  void destroy();

  SoundEffect loadSoundEffect(const std::string &filePath);
  Music loadMusic(const std::string &filePath);

private:
  bool _isInitialized = false;

  std::map<std::string, Mix_Chunk *> _effectMap;
  std::map<std::string, Mix_Music *> _musicMap;
};
} // namespace JEngine
