#ifndef SPHGRID_H
#define SPHGRID_H

#include <time.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <tbb/tbb.h>

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
    void AddParticle(FluidParticle* particle);
    std::vector<FluidParticle*> GetParticles(int i, int j, int k);
    std::vector<FluidParticle*> GetNeighborParticles(FluidParticle*);

private:

    int m_width;  // Uses i or x
    int m_height; // Uses j or y
    int m_depth;  // Uses k or z
    float m_cellSize; // radius of particle influence
    glm::vec3 m_gridMin;
    glm::vec3 m_gridMax;
    bool m_useGrid;

    // -- cells
    std::vector<std::vector<FluidParticle*>> m_cells;

    // Return the cell index
    int GetCellIdx(const glm::vec3&);
    int GetCellIdx(int i, int j, int k);
    glm::ivec3 GetCellCoord(const glm::vec3&);

};

#endif
