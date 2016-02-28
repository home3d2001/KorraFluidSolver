#include "fluidSolver.h"

// ---------------------------------------------------- //
// FluidParticle
// ---------------------------------------------------- //
FluidParticle::FluidParticle()
{

}

FluidParticle::FluidParticle(
    const glm::vec3& pos,
    const glm::vec3& vel,
    const glm::vec3& accel
    ) :m_pos(pos), m_vel(vel), m_accel(accel)
{

}

void
FluidParticle::Update()
{
    // @todo: We're just doing Euler integration for now
    m_vel = m_vel + m_accel;
    m_pos = m_pos + m_vel;
}

void
FluidParticle::AddForce(
    const glm::vec3& force
    )
{
    m_accel += force;
}

const glm::vec3&
FluidParticle::Position() const
{
    return m_pos;
}


// ---------------------------------------------------- //
// FluidSolver
// ---------------------------------------------------- //
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
                    new FluidParticle(
                        glm::vec3(x, y, z),
                        glm::vec3(0.f),
                        glm::vec3(0.0f, GRAVITY, 0.0f)
                        )
                    );
            }
        }
    }
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
