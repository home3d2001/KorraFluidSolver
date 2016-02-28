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
    for (float x = 0; x < m_particleDim.x; x += m_separation)
    {
        for (float y = 0; y < m_particleDim.y; y += m_separation)
        {
            for (float z = 0; z < m_particleDim.z; z += m_separation)
            {
                m_particles.push_back(
                    new FluidParticle(glm::vec3(x, y, z))
                    );
            }
        }
    }

    // -- Initalize gravity
    m_gravity = GRAVITY;
}

FluidSolver::~FluidSolver()
{
    for (auto &particle : m_particles)
    {
        if (particle != nullptr)
        {
            delete particle;
        }
    }
}

void
FluidSolver::Update()
{

}
