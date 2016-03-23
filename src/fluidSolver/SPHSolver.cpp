#include "SPHSolver.h"

// ---------------------------------------------------- //
// SPHGrid
// ---------------------------------------------------- //
SPHGrid::SPHGrid(
    std::vector<FluidParticle*> particles,
    int width,
    int height,
    int depth,
    float cellSize
    ) : m_width(width), m_height(height), m_depth(depth), m_cellSize(cellSize)
{
    cells.resize(m_width * m_height * m_depth);

    // for(FluidParticle* p : particles) {
    //     AddParticle(p);
    // }
}

void
SPHGrid::AddParticle(
    FluidParticle* particle
    )
{
    glm::vec3 m_pos = particle->Position();
    int i = m_pos[0] / m_cellSize;
    int j = m_pos[1] / m_cellSize;
    int k = m_pos[2] / m_cellSize;

    if (i > m_width || j > m_height || k > m_depth) {
        return;
    }

    int idx = GetCellIdx(i, j, k);
    cells[idx].push_back(particle);
}

int
SPHGrid::GetCellIdx(
    int i,
    int j,
    int k
    )
{
    return i + j * m_depth + k * m_width * m_height;
}

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
        5,
        5,
        5,
        1.0f
        );
}

SPHSolver::~SPHSolver()
{

}

void SPHSolver::Update()
{

}
