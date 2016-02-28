#ifndef FLUIDSOLVER_H
#define FLUIDSOLVER_H

#include <glm/glm.hpp>
#include <mathConstants.h>
#include <vector>

class FluidSolver
{
public:
    FluidSolver(
        const glm::vec3& containerDim,
        const glm::vec3& particleDim,
        const float& separation
        );

    const std::vector<glm::vec3>& ParticlePositions() const;

    // Called every frame
    virtual void Update();

protected:
    // -- Particle info
    glm::vec3 m_containerDim;
    glm::vec3 m_particleDim;
    float m_separation;

    std::vector<glm::vec3> m_particlePositions;
};

#endif /* FLUIDSOLVER_H */
