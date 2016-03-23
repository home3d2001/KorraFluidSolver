#ifndef SPHGRID_H
#define SPHGRID_H

#include <glm/glm.hpp>
#include <mathConstants.h>
#include <vector>
#include <time.h>
#include <iostream>

#include <fluidSolver/fluidParticle.h>


// ---------------------------------------------------- //
// SPHGrid
// ---------------------------------------------------- //
class SPHGrid
{
public:
    SPHGrid(
        const std::vector<FluidParticle*>& particles,
        const int width,
        const int height,
        const int depth,
        const float radius,
        const glm::vec3& gridMin,
        const glm::vec3& gridMax,
        const bool useGrid // Use grid to speed up neighbor search
        );
    void AddParticle(FluidParticle* particle);
    std::vector<FluidParticle*> GetParticles(int i, int j, int k);
    std::vector<FluidParticle*> GetNeighborParticles(FluidParticle*);

private:

    int m_width;  // i
    int m_height; // j
    int m_depth;  // k
    float m_radius; // radius of particle influence
    glm::vec3 m_gridMin;
    glm::vec3 m_gridMax;
    bool m_useGrid;

    // -- cells
    std::vector<std::vector<FluidParticle*>> m_cells;

    // Return the cell index
    int GetCellIdx(int i, int j, int k);
    int GetTopCellIdx(int& i, int& j, int& k);

};

#endif
