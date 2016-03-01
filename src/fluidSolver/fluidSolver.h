#ifndef FLUIDSOLVER_H
#define FLUIDSOLVER_H

#include <glm/glm.hpp>
#include <mathConstants.h>
#include <vector>

// ---------------------------------------------------- //
// FluidParticle
// ---------------------------------------------------- //
class FluidParticle
{
public:
    FluidParticle();
    FluidParticle(
        const glm::vec3& pos,
        const glm::vec3& vel
        );

    // Getters/setters
    const glm::vec3& Position() const { return m_pos; }
    const glm::vec3& Velocity() const { return m_vel; }
    const float& SpawnTime() const { return m_spawnTime; }

protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;
    float m_spawnTime = 0.0f;
};

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

    ~FluidSolver();
    const std::vector<glm::vec3> ParticlePositions() const;
    const std::vector<glm::vec3> ParticleVelocities() const;
    const std::vector<float> ParticleSpawnTimes() const;

    virtual void Update();

protected:

    // -- Particle info
    glm::vec3 m_containerDim;
    glm::vec3 m_particleDim;
    float m_separation;

    std::vector<FluidParticle*> m_particles;
};

#endif /* FLUIDSOLVER_H */
