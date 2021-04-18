#include "Level.h"

#include "../JEngine/Errors.h"
#include "../JEngine/ResourceManager.h"
#include <fstream>
#include <iostream>

Level::Level(const std::string &fileName) {
  std::ifstream file;

  file.open(fileName);

  // error checking
  if (file.fail()) {
    JEngine::fatalError("Failed to open level: " + fileName);
    return;
  }

  std::string tmp;
  file >> tmp >> _humans;

  // throw away the first line
  std::getline(file, tmp);

  while (std::getline(file, tmp)) {
    _levelData.push_back(tmp);
  }

  glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
  JEngine::ColorRGBA8 color;
  color.setColor(255, 255, 255, 255);

  _spriteBatch.init();
  _spriteBatch.begin();

  for (int y = 0; y < _levelData.size(); y++) {
    for (int x = 0; x < _levelData[y].size(); x++) {
      // get tile and dest rect
      char tile = _levelData[y][x];

      glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH,
                         TILE_WIDTH);

      // associate
      switch (tile) {
      case 'R':
        _spriteBatch.draw(
            destRect, uvRect,
            JEngine::ResourceManager::getTexture("Textures/red_bricks.png").id,
            0.0f, color);
        break;
      case 'G':
        _spriteBatch.draw(
            destRect, uvRect,
            JEngine::ResourceManager::getTexture("Textures/glass.png").id, 0.0f,
            color);
        break;
      case 'L':
        _spriteBatch.draw(
            destRect, uvRect,
            JEngine::ResourceManager::getTexture("Textures/light_bricks.png")
                .id,
            0.0f, color);
        break;
      case 'B':
        _spriteBatch.draw(
            destRect, uvRect,
            JEngine::ResourceManager::getTexture("Textures/bricks.png").id,
            0.0f, color);
        break;

      case '.':
        break;

      case '@':
        _startPlayerPos.x = x * TILE_WIDTH;
        _startPlayerPos.y = y * TILE_WIDTH;

        // prevent collidable
        _levelData[y][x] = '.';
        break;

      case 'Z':
        _zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
        _levelData[y][x] = '.';
        break;

      default:
        std::cout << "Unexpected character/tile read level: " << tile << ", "
                  << x << ", " << y << std::endl;
        break;
      }
    }
  }

  _spriteBatch.end();
}

Level::~Level() {}

void Level::draw() { _spriteBatch.renderBatch(); }
