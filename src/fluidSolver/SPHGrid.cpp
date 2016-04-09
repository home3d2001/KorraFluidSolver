#include "SPHGrid.h"

void
SPHGrid::InitializeVdb(
    const std::vector<FluidParticle*>& particles
    )
{
    openvdb::initialize();

    openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create(2.0);

    // CoordBBox indexBB(Coord(-10, -10, -10), Coord(10, 10, 10));
    CoordBBox indexBB(Coord(0, 0, 0), Coord(20, 20, 20));
    MakeWaterSurface(grid, FluidParticle::separation, indexBB, m_cellSize, particles);

    // specify dataset name
    grid->setName("WaterSurface");

    // save grid in the file
    openvdb::io::File file("fluidGeo.vdb");
    openvdb::GridPtrVec grids;
    grids.push_back(grid);
    file.write(grids);
    file.close();
}

void
SPHGrid::MakeWaterSurface(
    FloatGrid::Ptr grid,
    const float radius,
    const CoordBBox& indexBB,
    const double cellSize,
    const std::vector<FluidParticle*>& particles
    )
{
    double h = cellSize;
    // radius = 5.0f;
    // h = 0.5;
    typename FloatGrid::Accessor accessor = grid->getAccessor();

    for (Int32 i = indexBB.min().x(); i <= indexBB.max().x(); ++i) {
        for (Int32 j = indexBB.min().y(); j <= indexBB.max().y(); ++j) {
            for (Int32 k = indexBB.min().z(); k <= indexBB.max().z(); ++k) {

                // transform point (i, j, k) of index space into world space
                glm::vec3 vecP = GetWorlCoord(glm::ivec3(i, j, k));
                Vec3d p(vecP.x, vecP.y, vecP.z);

                // compute level set function value
                float distance = 9999;

                int cellIdx = GetCellIdx(i, j, k);
                std::vector<FluidParticle*> list = m_cells[cellIdx];

                for (FluidParticle* particle : list) {
                    float voxelToParticle = glm::distance(particle->Position(), vecP) - radius;
                    if (voxelToParticle < distance) {
                        distance = voxelToParticle;
                    }
                }
                if (distance < 9999) {
                    accessor.setValue(Coord(i, j, k), distance);
                }
            }
        }
    }

    grid->setTransform(openvdb::math::Transform::createLinearTransform(h));
}

// ---------------------------------------------------- //
// SPHGrid
// ---------------------------------------------------- //
SPHGrid::SPHGrid(
    const std::vector<FluidParticle*>& particles,
    const glm::vec3& gridMin,
    const glm::vec3& gridMax,
    const double cellSize,
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

    this->ResetGrid(particles);

    InitializeVdb(particles);
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
    glm::ivec3 coord = GetCellCoord(position);
    int i = coord.x;
    int j = coord.y;
    int k = coord.z;

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

glm::vec3
SPHGrid::GetWorlCoord(
    const glm::ivec3& cellCoord
    )
{
    return glm::vec3(
        cellCoord.x * m_cellSize + m_gridMin.x,
        cellCoord.y * m_cellSize + m_gridMin.y,
        cellCoord.z * m_cellSize + m_gridMin.z
        );
}

void
SPHGrid::UpdateNeighbors(
    FluidParticle* particle
    )
{
    particle->ResetNeighborCount();
    if (m_useGrid) {
        this->UpdateNeighborsUniformGrid(particle);
    } else {
        this->UpdateNeighborsSimple(particle);
    }
}


void
SPHGrid::UpdateNeighborsSimple(
    FluidParticle* particle
    )
{
    size_t len = m_cells[0].size();

    for (size_t i = 0; i < len; ++i) {
        FluidParticle* neighbor = m_cells[0][i];
        if (glm::distance(neighbor->Position(), particle->Position()) < m_cellSize &&
            neighbor != particle) {

            if(!particle->AddNeighbor(neighbor)) {
                // Can't add more neighbors to limit, stop loop
                break;
            }
        }
    }
}

void
SPHGrid::UpdateNeighborsUniformGrid(
    FluidParticle* particle
    )
{
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
                    if (glm::distance(neighbor->Position(), particlePosition) < m_cellSize * 2.0 &&
                        neighbor != particle) {

                        if(!particle->AddNeighbor(neighbor)) {
                            // Can't add more neighbors to limit, stop loop
                            break;
                        }
                    }
                }
            }
        }
    }
}
