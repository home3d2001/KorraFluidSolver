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

        // LOG(INFO) << "Number of particles: " << particles.size() << endl;
        // LOG(INFO) << "Grid dim: " << m_width << ", " << m_height << ", " << m_depth << endl;
    } else {
        // Grid size is only 1
        m_cells.resize(1);
    }

    this->ResetGrid(particles);
}

void
InitializeVdb()
{
    // // -- Initialize openvdb
    // openvdb::initialize();

    // // Create an empty floating-point grid with background value 0.
    // openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();

    // // Get an accessor for coordinate-based access to voxels.
    // openvdb::FloatGrid::Accessor accessor = grid->getAccessor();

}

void
SPHGrid::ResetGrid(
    const std::vector<FluidParticle*>& particles
    )
{
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
        if (idx > m_cells.size() || idx < 0) {
            // LOG(WARNING) << "Error adding particle " + idx << endl;
            return;
        }

        // If the particle isn't added to the cell yet, add it now
        if (std::find(m_cells[idx].begin(), m_cells[idx].end(),particle) == m_cells[idx].end()) {
            m_cells[idx].push_back(particle);
        }
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

    // -- Error checking
    if (i < 0 || j < 0 || k < 0 || i >= m_width || j >= m_height || k >= m_depth) {
        // LOG(WARNING) << "WARNING GetCellIdx "<< endl;
        // LOG(WARNING) << "x: "<< position.x << ", y: " << position.y << ", z: " << position.z << endl;
        // LOG(WARNING) << "i: "<< i << ", j: " << j << ", k: " << k << endl;
        return -1;
    }

    int idx = this->GetCellIdx(i, j, k);
    // LOG(DEBUG) << "i: "<< i << ", j: " << j << ", k: " << k << " -> " << idx << endl;
    return idx;
}

int
SPHGrid::GetCellIdx(
    int i,
    int j,
    int k
    )
{
    // int idx = i + j * m_depth + k * m_width * m_height;
    // if (idx < 0) {
    //     LOG(ERROR) << "Get cell idx is negative " + idx << endl;
    // }

    // -- Uses z-curve indexing
    // See https://en.wikipedia.org/wiki/Z-order_curve
    int i_temp = i;
    int j_temp = j;
    int k_temp = k;
    int idx = 0;
    int digit = 0;
    while (i_temp != 0 || j_temp != 0 || k_temp != 0) {
        // Interleave bits
        char i_bit = i_temp & 0x1;
        idx = idx | (i_bit << digit);
        digit++;
        i_temp = i_temp >> 1;

        char j_bit = j_temp & 0x1;
        idx = idx | (j_bit << digit);
        digit++;
        j_temp = j_temp >> 1;

        char k_bit = k_temp & 0x1;
        idx = idx | (k_bit << digit);
        digit++;
        k_temp = k_temp >> 1;
    }
    if (idx < 0 || idx > m_cells.size()) {
        // LOG(ERROR) << "Get cell idx is negative " + idx << endl;
        return -1;
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
SPHGrid::SearchNeighbors(
    FluidParticle* particle
    )
{
    if (m_useGrid) {
        return this->SearchNeighborsUniformGrid(particle);
    } else {
        return this->SearchNeighborsSimple(particle);
    }
}


std::vector<FluidParticle*>
SPHGrid::SearchNeighborsSimple(
    FluidParticle* particle
    )
{
    vector<FluidParticle*> neighbors;

    int len = m_cells[0].size();
    for (int i = 0; i < len; ++i) {
        FluidParticle* neighbor = m_cells[0][i];
        if (glm::distance(neighbor->Position(), particle->Position()) < m_cellSize &&
            neighbor != particle) {
            neighbors.push_back(neighbor);
        }
    };

    return neighbors;
}

std::vector<FluidParticle*>
SPHGrid::SearchNeighborsUniformGrid(
    FluidParticle* particle
    )
{
    vector<FluidParticle*> neighbors;

    // -- Use grid to search neighbor
    glm::ivec3 cellCoord = this->GetCellCoord(particle->Position());
    // Search in all 27 neighboring cells
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {

                // Check if any neighbor is out of bound
                if (cellCoord.x + i < 0 ||
                    cellCoord.y + j < 0 ||
                    cellCoord.z + k < 0 ||
                    cellCoord.x + i >= m_width ||
                    cellCoord.y + j >= m_height ||
                    cellCoord.z + k >= m_depth) {
                    continue;
                }

                int idx = this->GetCellIdx(i + cellCoord.x, j + cellCoord.y, k + cellCoord.z);
                glm::vec3 particlePosition = particle->Position();
                for(FluidParticle* neighbor : m_cells[idx]) {
                    if (glm::distance(neighbor->Position(), particlePosition) < m_cellSize * 2 &&
                        neighbor != particle) {
                        neighbors.push_back(neighbor);
                    }
                }
            }
        }
    }

    return neighbors;
}
