#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "Vertex.h"

namespace JEngine {
enum class GlyphSortType {
  NONE,
  FRONT_TO_BACK,
  BACK_TO_FRONT,
  TEXTURE // all of the same texture gets drawn
};

// a single sprite
class Glyph {
public:
  Glyph() {}
  Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture,
        float Depth, const ColorRGBA8 &color);
  Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture,
        float Depth, const ColorRGBA8 &color, float angle);

  GLuint texture;
  float depth;

  // 4, end number is 6
  Vertex topLeft;
  Vertex bottomLeft;
  Vertex topRight;
  Vertex bottomRight;

private:
  // rotates vertex arround point, angle in radians
  glm::vec2 rotatePoint(glm::vec2 pos, float angle);
};

class RenderBatch {
public:
  RenderBatch(GLuint nOffset, GLuint nnumVertices, GLuint nTexture)
      : offset(nOffset), numVertices(nnumVertices), texture(nTexture) {}
  GLuint offset;
  GLuint numVertices;
  GLuint texture;
};

// C# XNA equalivent! :D
class SpriteBatch {
public:
  // create, then initialize
  SpriteBatch();
  ~SpriteBatch();

  void init();

  // start draw
  void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

  // end draw
  void end();

  // add something to the batch
  // more like "add" then "draw"
  // Destination rectangle, x, y, width, height
  // const prevents making it a "pointer"
  void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture,
            float depth, const ColorRGBA8 &color);
  void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture,
            float depth, const ColorRGBA8 &color, float angle /*in radians*/);
  void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture,
            float depth, const ColorRGBA8 &color, const glm::vec2 &dir);

  // draw it finally
  void renderBatch();

private:
  void createRenderBatches();
  void createVertexArray();
  void sortGlyphs();

  static bool compareFrontToBack(Glyph *a, Glyph *b);
  static bool compareBackToFront(Glyph *a, Glyph *b);
  static bool compareTexture(Glyph *a, Glyph *b);

  GlyphSortType _sortType;

  GLuint _vbo;
  GLuint _vao;

  std::vector<Glyph *> _glyphPointers; // sorting
  std::vector<Glyph> _glyphs;          // actual glyphs
  std::vector<RenderBatch> _renderBatches;
};
} // namespace JEngine
