#ifndef PARTICLE_EMIT_PROGRAM_H
#define PARTICLE_EMIT_PROGRAM_H

#include <shaderProgram/shaderProgram.h>

class ParticleEmitProgram : public ShaderProgram
{
public:
    ParticleEmitProgram(
        const char* vertFilePath,
        const char* fragFilePath
        ) : ShaderProgram(vertFilePath, fragFilePath) {}

    virtual void Draw(
        const Camera&,
        const Geometry&
        ) const;
};

#endif
