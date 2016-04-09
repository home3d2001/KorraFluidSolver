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
#include <openvdb/tools/LevelSetSphere.h>
// #include <PointDataGrid.h>
// #include <PointConversion.h>
// #include <PointCount.h>
#include <thirdparty/easylogging++.h>

#include <mathConstants.h>
#include <fluidSolver/fluidParticle.h>

using namespace std;
using namespace tbb;
using namespace openvdb;

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
        const double cellSize,
        const bool useGrid // Use grid to speed up neighbor search
        );
    virtual void ResetGrid(const std::vector<FluidParticle*>& particles);
    virtual void AddParticle(FluidParticle* particle);
    virtual void UpdateNeighbors(
        FluidParticle*
    );

private:

    int m_width;  // Uses i or x
    int m_height; // Uses j or y
    int m_depth;  // Uses k or z
    double m_cellSize; // radius of particle influence
    glm::vec3 m_gridMin;
    glm::vec3 m_gridMax;
    bool m_useGrid;

    std::vector<std::vector<FluidParticle*>> m_cells;

    // -- Open vdb
    void InitializeVdb(const std::vector<FluidParticle*>& particles);
    void MakeWaterSurface(
        FloatGrid::Ptr grid,
        const float radius,
        const CoordBBox& indexBB,
        const double cellSize,
        const std::vector<FluidParticle*>& particles
        );


    // -- Cell idx/coordinates
    int GetCellIdx(const glm::vec3&);
    int GetCellIdx(int i, int j, int k);
    glm::ivec3 GetCellCoord(const glm::vec3& position);
    glm::vec3 GetWorlCoord(const glm::ivec3& cellCoord);

    // -- Neighbor search algorithms
    void UpdateNeighborsSimple(
        FluidParticle*
    );
    void UpdateNeighborsUniformGrid(
        FluidParticle*
    );
};

#endif
