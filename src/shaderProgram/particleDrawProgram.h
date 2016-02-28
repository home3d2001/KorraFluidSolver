#ifndef PARTICLE_DRAW_PROGRAM_H
#define PARTICLE_DRAW_PROGRAM_H

#include <shaderProgram/shaderProgram.h>

class ParticleDrawProgram : public ShaderProgram
{
public:
    ParticleDrawProgram(
        const char* vertFilePath,
        const char* fragFilePath
        ) : ShaderProgram(vertFilePath, fragFilePath) {}

    virtual void Draw(
        const Camera&,
        const Geometry&
        ) const;
};

#endif
