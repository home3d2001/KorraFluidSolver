#include "SPHGrid.h"

// ---------------------------------------------------- //
// SPHGrid
// ---------------------------------------------------- //
SPHGrid::SPHGrid(
    const std::vector<FluidParticle*>& particles,
    const glm::vec3& gridMin,
    const glm::vec3& gridMax,
    const float cellSize,
    const bool useGrid
    ) : m_width(1), m_height(1), m_depth(1),
        m_cellSize(cellSize), m_gridMin(gridMin), m_gridMax(gridMax),
        m_useGrid(useGrid)
{
    if (useGrid) {
        m_width = ceil((m_gridMax.x - m_gridMin.x) / m_cellSize);
        m_height = ceil((m_gridMax.y - m_gridMin.y) / m_cellSize);
        m_depth = ceil((m_gridMax.z - m_gridMin.z) / m_cellSize);
        m_cells.resize(m_width * m_height * m_depth);
    } else {
        // Grid size is only 1
        m_cells.resize(1);
    }

    for(FluidParticle* p : particles) {
        AddParticle(p);
    }
}

// Add particle to grid based on its position
void
SPHGrid::AddParticle(
    FluidParticle* particle
    )
{
    if (!m_useGrid) {
        // No grid, push particle in normally
        m_cells[0].push_back(particle);
    } else {
        // Add particle to grid based on its position
        int idx = GetCellIdx(particle->Position());
        if (idx > m_cells.size()) {
            cout << "Error adding particle " + idx << endl;
            return;
        }
        m_cells[idx].push_back(particle);
    }
}

int
SPHGrid::GetCellIdx(
    const glm::vec3& position
    )
{
    int i = (position.x - m_gridMin.x) / m_cellSize;
    int j = (position.y - m_gridMin.y) / m_cellSize;
    int k = (position.z - m_gridMin.z) / m_cellSize;

    return this->GetCellIdx(i, j, k);
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

std::vector<FluidParticle*>
SPHGrid::GetNeighborParticles(
    FluidParticle* particle
    )
{
    // @todo: Use naive neighbor search
    if (!m_useGrid) {
        int len = m_cells[0].size();
        parallel_for(0, len, [&](int i) {
            FluidParticle* neighbor = m_cells[0][i];
            if (glm::distance(neighbor->Position(), particle->Position()) < m_cellSize &&
                neighbor != particle) {

                // Change color of this neighbor
                neighbor->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            } else {
                neighbor->SetColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
            }
        });
    } else {

    }

    return neighbors;
}
