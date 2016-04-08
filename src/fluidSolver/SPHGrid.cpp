#include "SPHGrid.h"

// ---------------------------------------------------- //
// SPHGrid
// ---------------------------------------------------- //
SPHGrid::SPHGrid(
    std::vector<FluidParticle*>& particles,
    const glm::vec3& gridMin,
    const glm::vec3& gridMax,
    const float cellSize
    ) : m_width(1), m_height(1), m_depth(1),
        m_cellSize(cellSize), m_gridMin(gridMin), m_gridMax(gridMax)
{
    // -- Initialize uniform grid
    m_width = ceil((m_gridMax.x - m_gridMin.x) / m_cellSize);
    m_height = ceil((m_gridMax.y - m_gridMin.y) / m_cellSize);
    m_depth = ceil((m_gridMax.z - m_gridMin.z) / m_cellSize);
    m_cells.resize(m_width * m_height * m_depth);

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

void Insert(
    std::vector<FluidParticle*>& list,
    int index,
    FluidParticle* particle
    )
{
    // Shift values over
    int i;
    for (i = index; i >= 0, particle->m_cellIdx <= list[i]->m_cellIdx; --i) {
        list[i + 1] = list[i];
    }
    // Insert particle in the correct place
    list[i + 1] = particle;
}

void InsertionSort(
    std::vector<FluidParticle*>& list
    )
{
    int len = list.size();
    for (int i = 1; i < len; ++i) {
        Insert(list, i - 1, list[i]);
    }
}

void
SPHGrid::ResetGrid(
    std::vector<FluidParticle*>& particles
    )
{
    // -- Set cell index for each particle
    int len = particles.size();
    parallel_for(0, len, [&](int particleIdx) {

        FluidParticle* particle = particles[particleIdx];

        // Add particle to grid based on its position
        int cellIdx = GetCellIdx(particle->Position());
        if (cellIdx < m_cells.size() && cellIdx >= 0) {
            particle->m_cellIdx = cellIdx;
        }
    });

    // -- Sort particles based on cell index
    // InsertionSort(particles);

    // -- Initialize cell idx
    len = m_cells.size();
    parallel_for(0, len, [&](int cellIdx) {
        m_cells[cellIdx] = std::make_pair(-1, 0); // -1 for invalid
    });

    // -- Update cell values based on sorted particle list
    len = particles.size();
    for (int particleIdx = 0; particleIdx < len; ++particleIdx) {

        int cellIdx = particles[particleIdx]->m_cellIdx;
        if (m_cells[cellIdx].first = -1) {
            // If there are no particles yet in the cell, add the first one
            m_cells[cellIdx].first = particleIdx;
            if (particleIdx == 0) {

            }
        }

        ++m_cells[cellIdx].second;
    }

    for (int i = 0; i < m_cells.size(); ++i) {
        if (m_cells[i].first != -1) {
            LOG(INFO) << "cell #" << to_string(i) << ", first particle: " << to_string(m_cells[i].first) << ", # particles: " << to_string(m_cells[i].second) << endl;
        }
    }

    // -- Update neighbors
    len = particles.size();
    for(int i = 0; i < len; ++i) {
        // LOG(DEBUG) << "neighbor: " << particles[i]->m_cellIdx << endl;
        // LOG(INFO) << "1: " << to_string(m_cells[cellIdx].first) << ", 2: " << to_string(m_cells[cellIdx].second) << endl;

        // this->UpdateNeighbors(particles[i], particles);
    };
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
        return -1;
    }

    int idx = this->GetCellIdx(i, j, k);
    return idx;
}

int
SPHGrid::GetCellIdx(
    int i,
    int j,
    int k
    )
{
#define USE_Z_CURVE
#ifndef USE_Z_CURVE
    int idx = i + j * m_depth + k * m_width * m_height;
    if (idx < 0) {
        LOG(ERROR) << "Get cell idx is negative " + idx << endl;
    }
#else
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
#endif
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

void
SPHGrid::UpdateNeighbors(
    FluidParticle* particle,
    const std::vector<FluidParticle*>& particles
    )
{
    particle->ResetNeighborCount();
    glm::vec3 particlePosition = particle->Position();

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

                int cellIdx = this->GetCellIdx(i + cellCoord.x, j + cellCoord.y, k + cellCoord.z);
                static bool colored = false;

                // Unpack particles stored
                int firstParticleIdx = m_cells[cellIdx].first;
                int numParticlesInCell = m_cells[cellIdx].second;
                for (int offset = 0; offset < numParticlesInCell; ++offset) {
                    FluidParticle* neighbor = particles[firstParticleIdx + offset];


                    if (glm::distance(neighbor->Position(), particlePosition) < m_cellSize * 2 &&
                        neighbor != particle) {

                        if (particle->m_cellIdx == 10 & !colored) {
                            // neighbor->SetColor(glm::vec4(1.0, 0, 0, 1));
                        }

                        // if(!particle->AddNeighbor(neighbor)) {
                        //     // Can't add more neighbors to limit, stop loop
                        //     break;
                        // }
                    }
                }

                if (particle->m_cellIdx == 10) {
                    colored = true;
                }
            }
        }
    }
}
