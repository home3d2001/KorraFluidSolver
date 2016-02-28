#ifndef FLUIDSOLVER_H
#define FLUIDSOLVER_H

#include <glm/glm.hpp>
#include <mathConstants.h>
#include <vector>

class FluidParticle
{
public:
    FluidParticle() {};
    FluidParticle(const glm::vec3& position) : m_position(position) {};

    void Translate(
        const glm::vec3& offset
        )
    {
        m_position += offset;
    }

    const glm::vec3& Position() const
    {
        return m_position;
    }
private:
    glm::vec3 m_position;
};


class FluidSolver
{
public:
    FluidSolver(
        const glm::vec3& containerDim,
        const glm::vec3& particleDim,
        const float& separation
        );
    ~FluidSolver();
    void Update();
private:

    // -- Particle info
    glm::vec3 m_containerDim;
    glm::vec3 m_particleDim;
    float m_separation;

    // -- Forces
    float m_gravity;

    std::vector<FluidParticle*> m_particles;
};

#endif /* FLUIDSOLVER_H */
