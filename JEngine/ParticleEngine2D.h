#pragma once

#include <vector>

namespace JEngine {
class ParticleBatch2D;
class SpriteBatch;

class ParticleEngine2D {
public:
  ParticleEngine2D();
  ~ParticleEngine2D();

  // after adding a particle batch, the ParticleEngine2D becomes
  // responsible for disposing resources
  void addParticleBatch(ParticleBatch2D *particleBatch);

  void update(float deltaTime);
  void draw(SpriteBatch *spriteBatch);

private:
  std::vector<ParticleBatch2D *> _batches;
};

} // namespace JEngine
