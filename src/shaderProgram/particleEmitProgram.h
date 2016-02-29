#ifndef PARTICLE_EMIT_PROGRAM_H
#define PARTICLE_EMIT_PROGRAM_H

#include <shaderProgram/shaderProgram.h>

class FluidGeo;

class ParticleEmitProgram : public ShaderProgram
{
public:
    ParticleEmitProgram(
        const char* vertFilePath,
        const char* fragFilePath
        );

    virtual void Draw(
        const Camera*,
        const FluidGeo*
        ) const;

protected:

    GLuint m_transformFeedback;

    // Uniform locations
    int m_unifTime;
    int m_unifAccel;
};

#endif
