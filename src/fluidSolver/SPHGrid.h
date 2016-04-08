#ifndef SPHGRID_H
#define SPHGRID_H

#include <time.h>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
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
        std::vector<FluidParticle*>& particles,
        const glm::vec3& gridMin,
        const glm::vec3& gridMax,
        const float cellSize
        );
    virtual void ResetGrid(std::vector<FluidParticle*>& particles);
    virtual void UpdateNeighbors(
        FluidParticle* p,
        const std::vector<FluidParticle*>& particles
    );

private:

    int m_width;  // Uses i or x
    int m_height; // Uses j or y
    int m_depth;  // Uses k or z
    float m_cellSize; // radius of particle influence
    glm::vec3 m_gridMin;
    glm::vec3 m_gridMax;

    // -- Particles are sorted using index of the first particle in that cell
    // and the number of particles in the same cell
    // The format of pair is <first particle index, number of particles in that cell stored consequetively>
    std::vector<std::pair<int, int>> m_cells;

    // -- Cell idx/coordinates
    int GetCellIdx(const glm::vec3&);
    int GetCellIdx(int i, int j, int k);
    glm::ivec3 GetCellCoord(const glm::vec3&);
};

#endif
