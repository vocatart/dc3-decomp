#include "rndobj/SoftParticles.h"
#include "rndobj/BaseMaterial.h"

RndSoftParticles::RndSoftParticles()
    : mParticles(this), mBlend(BaseMaterial::kBlendSrcAlphaAdd) {}
