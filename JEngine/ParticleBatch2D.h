#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "GLTexture.h"
#include "SpriteBatch.h"
#include "Vertex.h"


namespace JEngine {
class Particle2D {
public:
  glm::vec2 pos = glm::vec2(0.0f);
  glm::vec2 vel = glm::vec2(0.0f);
  ColorRGBA8 color;
  float lifeTime = 1.0f;
  float width = 0.0f;
};

inline void defaultParticleUpdate(Particle2D &part, float deltaTime) {
  part.pos += part.vel * deltaTime;
}

class ParticleBatch2D {
public:
  ParticleBatch2D();
  ~ParticleBatch2D();

  void init(unsigned int maxParticles, float decayRate, GLTexture texture,
            std::function<void(Particle2D &, float)> updateFunc =
                defaultParticleUpdate);

  void update(float deltaTime);
  void draw(SpriteBatch *spriteBatch);

  void addParticle(const glm::vec2 &position, const glm::vec2 &velocity,
                   const ColorRGBA8 &color, const float &width);

private:
  std::function<void(Particle2D &, float)> _updateFunc;

  int findFreeParticle();

  float _decayRate = 0.1f;
  unsigned int _maxParticles = 0;
  int _nLastFreeParticle = 0;

  // dynamic array
  Particle2D *_particles = nullptr;

  GLTexture _texture;
};

} // namespace JEngine
