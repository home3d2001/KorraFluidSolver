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
    Advect();
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


// -- Private -- //
void
FluidParticle::Advect()
{
    // @todo: We're just doing Euler integration for now
    // RK2 soon
    m_vel = m_vel + m_accel;
    m_pos = m_pos + m_vel;

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
    float midX = m_particleDim.x / 2.f;
    float midY = m_particleDim.y / 2.f;
    float midZ = m_particleDim.z / 2.f;
    for (float x = -midX; x < midX; x += m_separation)
    {
        for (float y = -midY; y < midY; y += m_separation)
        {
            for (float z = -midZ; z < midX; z += m_separation)
            {
                m_particles.push_back(
                    new FluidParticle(
                        glm::vec3(x, y, z),
                        glm::vec3(0.f, 0.0001f, 0.0f),
                        glm::vec3(0.0f, GRAVITY * 0.0001f, 0.0f)
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

const std::vector<glm::vec3>
FluidSolver::ParticlePositions()
{
    std::vector<glm::vec3> positions;
    for (auto &particle : m_particles)
    {
        positions.push_back(particle->Position());
    }

    return positions;
}

void
FluidSolver::Update()
{
    for (auto &particle : m_particles)
    {
        particle->Update();
    }
}
