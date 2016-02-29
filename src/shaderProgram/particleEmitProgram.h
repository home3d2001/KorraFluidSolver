#ifndef PARTICLE_EMIT_PROGRAM_H
#define PARTICLE_EMIT_PROGRAM_H

#include <loadShaders.h>
#include <camera/camera.h>
#include <geometry/fluidGeo.h>

class FluidGeo;

class ParticleEmitProgram
{
public:
    ParticleEmitProgram(
        const char* vertEmitFilepath,
        const char* fragEmitFilepath,
        const char* vertDrawFilepath,
        const char* fragDrawFilepath
        );

    // First pass
    virtual void Emit(
        FluidGeo*
        );

    // Second pass
    virtual void Draw(
        const Camera*,
        const FluidGeo*
        );

    virtual void CleanUp();

protected:
    GLuint m_programEmit;
    GLuint m_programDraw;

    // Transform feedback buffer
    GLuint m_TFBuffers[2];
    char m_curTFBuffer = 0;

    // Uniform locations for emit program
    int m_unifEmitTime;
    int m_unifEmitAccel;

    // Uniform locations for draw program
    int m_unifDrawTime;
    int m_unifDrawColor;
    int m_unifDrawModel;
    int m_unifDrawViewProj;
};

#endif
