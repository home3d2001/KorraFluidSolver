#include "fluidSolver.h"

FluidSolver::FluidSolver(
    const glm::vec3& containerDim,
    const glm::vec3& particleDim,
    const float& separation
    ) : m_containerDim(containerDim),
        m_particleDim(particleDim),
        m_separation(separation)
{
    // -- Initialize all particles
    float midX = m_particleDim.x / 2.f;
    float midY = m_particleDim.y / 2.f;
    float midZ = m_particleDim.z / 2.f;
    for (float x = -midX; x < midX; x += m_separation)
    {
        for (float y = -midY; y < midY; y += m_separation)
        {
            for (float z = -midZ; z < midX; z += m_separation)
            {
                m_particlePositions.push_back(
                    glm::vec3(x, y, z)
                    );
            }
        }
    }
}

const std::vector<glm::vec3>&
FluidSolver::ParticlePositions() const
{
    return m_particlePositions;
}

void
FluidSolver::Update()
{
}
