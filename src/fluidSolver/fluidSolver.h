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
        const glm::vec3& vel,
        const glm::vec3& accel
        );

    virtual void Update();
    void AddForce(const glm::vec3& force);

    // Getters/setters
    const glm::vec3& Position() const;

protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;
    glm::vec3 m_accel;
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
    virtual void Update();

protected:
    // -- Particle info
    glm::vec3 m_containerDim;
    glm::vec3 m_particleDim;
    float m_separation;

    std::vector<FluidParticle*> m_particles;
};

#endif /* FLUIDSOLVER_H */
