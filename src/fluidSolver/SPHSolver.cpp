#include "SPHSolver.h"

// ---------------------------------------------------- //
// SPHSolver
// ---------------------------------------------------- //
SPHSolver::SPHSolver(
    const glm::vec3& containerDim,
    const glm::vec3& particleDim,
    const float separation,
    const float cellSize
    ) : FluidSolver(containerDim, particleDim, separation)
{
    m_grid = new SPHGrid(
        m_particles,
        -containerDim,
        containerDim,
        cellSize,
        true
        );
}

SPHSolver::~SPHSolver()
{
    delete m_grid;
}

void
SPHSolver::Update(
    const float deltaT
    )
{
    SearchNeighbors();
    CalculateDensity();
    CalculatePressure();
    Advect(deltaT);
}

void
SPHSolver::SearchNeighbors()
{
    static int i = 0;
    m_grid->SearchNeighbors(m_particles[i]);
    i = (i + 1) % m_particles.size();
}

void
SPHSolver::CalculateDensity()
{
}

void
SPHSolver::CalculatePressure()
{
}

void
SPHSolver::Advect(
    const float deltaT
    )
{
    for (FluidParticle* p : m_particles) {

        // Check boundary
        glm::vec3 newForce(0.0f);
        if (p->Position().x < m_minBoundary.x || p->Position().x > m_maxBoundary.x) {
            newForce.x = -p->Velocity().x;
        }

        if (p->Position().y < m_minBoundary.y) {
            newForce.y = 0.2f;
        } else if (p->Position().y > m_maxBoundary.y) {
            newForce.y = -0.0f;
        }

        if (p->Position().z < m_minBoundary.z || p->Position().z > m_maxBoundary.z) {
            newForce.z = -p->Velocity().z;
        }

        p->SetForce(newForce);

        // Update kinematic
        p->Update(deltaT);
    }
}
