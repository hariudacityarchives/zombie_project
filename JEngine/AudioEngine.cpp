#include "AudioEngine.h"
#include "Errors.h"

namespace JEngine {
void SoundEffect::play(int loops) {
  if (Mix_PlayChannel(-1, _chunk, loops) == -1) {
    // if both go wrong, something is really wrong
    // otherwise minor stuff like channels ran out that doesn't really matter
    if (Mix_PlayChannel(0, _chunk, loops)) {
      fatalError("SDL_mixer: Mix_PlayChannel error @ SoundEffect.play: " +
                 std::string(Mix_GetError()));
    }
  }
}

void Music::play(int times) {
  if (Mix_PlayMusic(_music, times) == -1) {
    fatalError("SDL_mixer: Mix_PlayMusic error @ Music.play: " +
               std::string(Mix_GetError()));
  }
}

void Music::pause() { Mix_PauseMusic(); }

void Music::stop() { Mix_HaltMusic(); }

void Music::resume() { Mix_ResumeMusic(); }

AudioEngine::AudioEngine() {}

AudioEngine::~AudioEngine() { destroy(); }

// Must be called after SDL init!
void AudioEngine::init(int flags) {
  if (_isInitialized) {
    fatalError("What where you thinking? Trying to initialize the Audio Engine "
               "twice?! @ AudioEngine.init(int flags)");
  }

  // Parameters can be a bitwise combination of MIX_INIT_FAC, MIX_INIT_MOD,
  // MIX_INIT_MP3, MIX_INIT_OGG
  if (Mix_Init(flags) == -1) {
    fatalError("SDL_mixer: Mix_Init error: " + std::string(Mix_GetError()));
  }

  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2 /*stereo*/, 1024);

  _isInitialized = true;
}

void AudioEngine::destroy() {
  if (_isInitialized) {
    _isInitialized = false;

    // and free the resources
    for (auto &it : _effectMap) {
      Mix_FreeChunk(it.second);
    }

    for (auto &it : _musicMap) {
      Mix_FreeMusic(it.second);
    }

    _effectMap.clear();
    _musicMap.clear();

    // close
    Mix_CloseAudio();
    Mix_Quit();
  }
}

SoundEffect AudioEngine::loadSoundEffect(const std::string &filePath) {
  // Try to find the audio in the cache
  auto it = _effectMap.find(filePath);

  SoundEffect effect;

  if (it == _effectMap.end()) {
    // failed to find, load
    Mix_Chunk *chunk = Mix_LoadWAV(filePath.c_str());

    // error
    if (chunk == nullptr) {
      fatalError("SDL_mixer: Mix_LoadWAV error: " +
                 std::string(Mix_GetError()));
    }

    effect._chunk = chunk;

    _effectMap[filePath] = chunk;
  } else {
    // already cached
    effect._chunk = it->second;
  }

  return effect;
}

Music AudioEngine::loadMusic(const std::string &filePath) {
  // Try to find the audio in the cache
  auto it = _musicMap.find(filePath);

  Music music;

  if (it == _musicMap.end()) {
    // failed to find, load
    Mix_Music *mixMusic = Mix_LoadMUS(filePath.c_str());

    // error
    if (mixMusic == nullptr) {
      fatalError("SDL_mixer: Mix_LoadMUS error: " +
                 std::string(Mix_GetError()));
    }

    music._music = mixMusic;

    _musicMap[filePath] = mixMusic;
  } else {
    // already cached
    music._music = it->second;
  }

  return music;
}

} // namespace JEngine