#ifndef SPHGRID_H
#define SPHGRID_H

#include <time.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <tbb/tbb.h>
#include <openvdb/openvdb.h>
#include <thirdparty/easylogging++.h>

#include <mathConstants.h>
#include <fluidSolver/fluidParticle.h>

using namespace std;
using namespace tbb;

// ---------------------------------------------------- //
// SPHGrid
// ---------------------------------------------------- //
class SPHGrid
{
public:
    SPHGrid(
        const std::vector<FluidParticle*>& particles,
        const glm::vec3& gridMin,
        const glm::vec3& gridMax,
        const float cellSize,
        const bool useGrid // Use grid to speed up neighbor search
        );
    virtual void ResetGrid(const std::vector<FluidParticle*>& particles);
    virtual void AddParticle(FluidParticle* particle);
    virtual std::vector<FluidParticle*> SearchNeighbors(FluidParticle*);

private:

    int m_width;  // Uses i or x
    int m_height; // Uses j or y
    int m_depth;  // Uses k or z
    float m_cellSize; // radius of particle influence
    glm::vec3 m_gridMin;
    glm::vec3 m_gridMax;
    bool m_useGrid;

    std::vector<std::vector<FluidParticle*>> m_cells;

    // -- Cell idx/coordinates
    int GetCellIdx(const glm::vec3&);
    int GetCellIdx(int i, int j, int k);
    glm::ivec3 GetCellCoord(const glm::vec3&);

    // -- Neighbor search algorithms
    std::vector<FluidParticle*> SearchNeighborsSimple(FluidParticle*);
    std::vector<FluidParticle*> SearchNeighborsUniformGrid(FluidParticle*);
};

#endif
