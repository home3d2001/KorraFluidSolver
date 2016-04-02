#include "fluidSolver.h"

// ---------------------------------------------------- //
// FluidSolver
// ---------------------------------------------------- //
FluidSolver::FluidSolver(
    const glm::vec3& containerDim,
    const glm::vec3& particleDim,
    const float separation,
    const float mass
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
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec4(0.3f, 0.3f, 1.0f, 1.0f)
                        )
                    );
            }
        }
    }

    // -- Set container boundary
    m_minBoundary = -m_containerDim / 2.0f;
    m_maxBoundary = m_containerDim / 2.0f;

    // -- Set particle properties
    FluidParticle::mass = mass;
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
FluidSolver::ParticlePositions() const
{
    std::vector<glm::vec3> positions;
    for (auto &particle : m_particles)
    {
        positions.push_back(particle->Position());
    }

    return positions;
}

const std::vector<glm::vec3>
FluidSolver::ParticleVelocities() const
{
    std::vector<glm::vec3> velocities;
    for (auto &particle : m_particles)
    {
        velocities.push_back(particle->Velocity());
    }

    return velocities;
}

const std::vector<float>
FluidSolver::ParticleSpawnTimes() const
{
    std::vector<float> spawnTimes;
    for (auto &particle : m_particles)
    {
        spawnTimes.push_back(particle->SpawnTime());
    }

    return spawnTimes;
}

const std::vector<glm::vec4>
FluidSolver::ParticleColors() const
{
    int len = m_particles.size();
    std::vector<glm::vec4> colors;
    colors.resize(len);
    for (int i = 0; i < len; ++i) {
        FluidParticle* particle = m_particles[i];
        colors.insert(colors.begin() + i, particle->Color());
    }

    return colors;
}

void
FluidSolver::Update(
    const float deltaT
    )
{
}
