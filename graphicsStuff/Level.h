#pragma once

#include <string>
#include <vector>

#include "../JEngine/SpriteBatch.h"

const unsigned short TILE_WIDTH = 64;

class Level {
public:
  // load the levelh
  Level(const std::string &fileName);
  ~Level();

  void draw();

  // Getters
  int getWidth() const { return _levelData[0].size(); }
  int getHeight() const { return _levelData.size(); }
  glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
  unsigned int getNumHumans() const { return _humans; }
  const std::vector<glm::ivec2> &getZombieStartPositions() const {
    return _zombieStartPositions;
  }
  const std::vector<std::string> &getLevelData() const { return _levelData; }

private:
  std::vector<std::string> _levelData;
  unsigned int _humans;

  JEngine::SpriteBatch _spriteBatch;

  // integer vector
  glm::ivec2 _startPlayerPos;
  std::vector<glm::ivec2> _zombieStartPositions;
};
