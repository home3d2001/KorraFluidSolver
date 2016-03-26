#include "SPHSolver.h"

// ---------------------------------------------------- //
// SPHSolver
// ---------------------------------------------------- //
SPHSolver::SPHSolver(
    const glm::vec3& containerDim,
    const glm::vec3& particleDim,
    const float& separation
    ) : FluidSolver(containerDim, particleDim, separation)
{
    m_grid = new SPHGrid(
        m_particles,
        -containerDim,
        containerDim,
        1.0f,
        false
        );

    // Initialize random seed
    srand (time(NULL));
}

SPHSolver::~SPHSolver()
{
    delete m_grid;
}

void
SPHSolver::Update()
{
    SearchNeighbors();
    CalculateDensity();
    CalculatePressure();
}

void
SPHSolver::SearchNeighbors()
{
    static int i = 0;
    m_grid->GetNeighborParticles(m_particles[i]);
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
