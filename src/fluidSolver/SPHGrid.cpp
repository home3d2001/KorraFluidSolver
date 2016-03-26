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
            cerr << "Error adding particle " + idx << endl;
            throw idx;
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
    int i = abs(position.x - m_gridMin.x) / m_cellSize;
    int j = abs(position.y - m_gridMin.y) / m_cellSize;
    int k = abs(position.z - m_gridMin.z) / m_cellSize;

    return this->GetCellIdx(i, j, k);
}

int
SPHGrid::GetCellIdx(
    int i,
    int j,
    int k
    )
{
    int idx = i + j * m_depth + k * m_width * m_height;
    if (idx < 0) {
        cerr << "Get cell idx is negative " + idx << endl;
        throw idx;
    }
    return idx;
}

glm::ivec3
SPHGrid::GetCellCoord(
    const glm::vec3& position
    )
{
    return glm::ivec3(
        (position.x - m_gridMin.x) / m_cellSize,
        (position.y - m_gridMin.y) / m_cellSize,
        (position.z - m_gridMin.z) / m_cellSize
        );
}

std::vector<FluidParticle*>
SPHGrid::GetNeighborParticles(
    FluidParticle* particle
    )
{
    vector<FluidParticle*> neighbors;

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

        // -- Use grid to search neighbor
        glm::ivec3 cellCoord = this->GetCellCoord(particle->Position());
        // Search in all 27 neighboring cells
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                for (int k = -1; k <= 1; ++k) {

                    // Check if any neighbor is out of bound
                    if (cellCoord.x + i < 0 || cellCoord.y + j < 0 || cellCoord.z + k < 0) {
                        continue;
                    }

                    int idx = this->GetCellIdx(i + cellCoord.x, j + cellCoord.y, k + cellCoord.z);
                    for(FluidParticle* neighbor : m_cells[idx]) {
                        if (glm::distance(neighbor->Position(), particle->Position()) < m_cellSize &&
                            neighbor != particle) {

                            // Change color of this neighbor
                            neighbor->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
                        } else {
                            neighbor->SetColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
                        }
                    }
                }
            }
        }
    }

    return neighbors;
}
