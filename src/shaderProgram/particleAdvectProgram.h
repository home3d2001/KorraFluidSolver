#ifndef PARTICLE_ADVECT_PROGRAM_H
#define PARTICLE_ADVECT_PROGRAM_H

#include <loadShaders.h>
#include <camera/camera.h>
#include <geometry/fluidGeo.h>
#include <geometry/box.h>

class FluidGeo;

class ParticleAdvectProgram
{
public:
    ParticleAdvectProgram(
        const char* vertAdvectFilepath,
        const char* fragAdvectFilepath,
        const char* vertDrawFilepath,
        const char* fragDrawFilepath
        );

    // First pass
    virtual void Advect(
        const float deltaTime,
        FluidGeo*
        );

    // Second pass
    virtual void Draw(
        const Camera*,
        const FluidGeo*,
        const Box*
        );

    virtual void CleanUp();

protected:
    GLuint m_programAdvect;
    GLuint m_programDraw;

    // Transform feedback buffer
    GLuint m_TFBuffers[2];
    char m_curTFBuffer = 0;

    // Uniform locations for advect program
    int m_unifAdvectTime;
    int m_unifAdvectAccel;

    // Uniform locations for draw program
    int m_unifDrawTime;
    int m_unifDrawColor;
    int m_unifDrawModel;
    int m_unifDrawViewProj;
    int m_unifDrawMinBoundary;
    int m_unifDrawMaxBoundary;

    // First draw
    bool m_isFirstDraw = true;
};

#endif
