#include "ParticleBatch2D.h"

namespace JEngine {
ParticleBatch2D::ParticleBatch2D() {}

ParticleBatch2D::~ParticleBatch2D() { delete[] _particles; }

void ParticleBatch2D::init(unsigned int maxParticles, float decayRate,
                           GLTexture texture,
                           std::function<void(Particle2D &, float)>
                               updateFunc /* = defaultParticleUPdate */) {
  _maxParticles = maxParticles;
  _particles = new Particle2D[maxParticles];

  _decayRate = decayRate;
  _texture = texture;

  _updateFunc = updateFunc;
}

void ParticleBatch2D::addParticle(const glm::vec2 &position,
                                  const glm::vec2 &velocity,
                                  const ColorRGBA8 &color, const float &width) {
  // find an inactive particle in the array, and set it to active.
  int n = findFreeParticle();

  auto &p = _particles[n];

  p.lifeTime = 1.0f;
  p.pos = position;
  p.vel = velocity;
  p.color = color;
  p.width = width;
}

void ParticleBatch2D::update(float deltaTime) {
  float lifeMin = _decayRate * deltaTime;

  for (int i = 0; i < _maxParticles; i++) {
    // check if active
    if (_particles[i].lifeTime > 0.0f) {
      _updateFunc(_particles[i], deltaTime);
      _particles[i].lifeTime -= lifeMin;
    }
  }
}

void ParticleBatch2D::draw(SpriteBatch *spriteBatch) {
  static glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

  for (int i = 0; i < _maxParticles; i++) {
    auto &p = _particles[i];
    if (p.lifeTime > 0.0f) {
      glm::vec4 destRect(p.pos.x, p.pos.y, p.width, p.width);
      spriteBatch->draw(destRect, uvRect, _texture.id, 0.0f, p.color);
    }
  }
}

int ParticleBatch2D::findFreeParticle() {
  for (int i = _nLastFreeParticle; i < _maxParticles; i++) {
    if (_particles[i].lifeTime <= 0.0f) {
      _nLastFreeParticle = i;
      return i;
    }
  }

  for (int i = 0; i < _nLastFreeParticle; i++) {
    if (_particles[i].lifeTime <= 0.0f) {
      _nLastFreeParticle = i;
      return i;
    }
  }

  // No particles, are free, so pretend first one is free
  return 0;
}

} // namespace JEngine
