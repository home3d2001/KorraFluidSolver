#include "SPHGrid.h"

// ---------------------------------------------------- //
// SPHGrid
// ---------------------------------------------------- //
SPHGrid::SPHGrid(
    const std::vector<FluidParticle*>& particles,
    const int width,
    const int height,
    const int depth,
    const float radius,
    const glm::vec3& gridMin,
    const glm::vec3& gridMax,
    const bool useGrid
    ) : m_width(width), m_height(height), m_depth(depth),
        m_radius(radius), m_gridMin(gridMin), m_gridMax(gridMax),
        m_useGrid(useGrid)
{
    if (useGrid) {
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

        // No grid
        m_cells[0].push_back(particle);
    } else {

        // Add to grid based on its position
        glm::vec3 m_pos = particle->Position();
        int i = m_pos[0] / m_radius;
        int j = m_pos[1] / m_radius;
        int k = m_pos[2] / m_radius;

        if (i > m_width || j > m_height || k > m_depth) {
            return;
        }

        int idx = GetCellIdx(i, j, k);
        if (idx > m_cells.size()) {
            std::cout << idx << std::endl;
            return;
        }
        m_cells[idx].push_back(particle);
    }
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
    std::vector<FluidParticle*> neighbors;

    // @todo: Use naive neighbor search
    if (!m_useGrid) {
        for (FluidParticle* neighbor : m_cells[0]) {
            if (glm::distance(neighbor->Position(), particle->Position()) < m_radius &&
                neighbor != particle) {

                // Change color of this neighbor
                neighbor->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
                neighbors.push_back(neighbor);
            } else {
                neighbor->SetColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
            }
        }
    } else {

    }

    return neighbors;
}
