#ifndef FLUIDSOLVER_H
#define FLUIDSOLVER_H

#include <glm/glm.hpp>
#include <mathConstants.h>
#include <vector>
#include <iostream>
#include <random>
#include <time.h>

#include <fluidSolver/fluidParticle.h>

// ---------------------------------------------------- //
// FluidSolver
// ---------------------------------------------------- //
class FluidSolver
{
public:
    FluidSolver(
        const glm::vec3& containerDim,
        const glm::vec3& particleDim,
        const float& separation
        );

    virtual ~FluidSolver();
    const std::vector<glm::vec3> ParticlePositions() const;
    const std::vector<glm::vec3> ParticleVelocities() const;
    const std::vector<float> ParticleSpawnTimes() const;
    const std::vector<glm::vec4> ParticleColors() const;

    virtual void Update(const float deltaT);

protected:

    // -- Particle info
    glm::vec3 m_containerDim;
    glm::vec3 m_particleDim;
    glm::vec3 m_minBoundary;
    glm::vec3 m_maxBoundary;
    float m_separation;

    std::vector<FluidParticle*> m_particles;
};

#endif /* FLUIDSOLVER_H */
