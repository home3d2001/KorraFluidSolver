#ifndef SPHGRID_H
#define SPHGRID_H

#include <time.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <utility>
#include <algorithm>
#include <glm/glm.hpp>
#include <tbb/tbb.h>
#include <openvdb/openvdb.h>

#ifdef OPENVDB_POINTS
#include <openvdb_points/openvdb.h>
#include <openvdb_points/tools/PointDataGrid.h>
#include <openvdb_points/tools/PointConversion.h>
#include <openvdb_points/tools/PointCount.h>
#endif

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
        const double cellSize
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

    // -- Grid
    // -- Particles are sorted using index of the first particle in that cell
    // and the number of particles in the same cell
    // The format of pair is <first particle index, number of particles in that cell stored consequetively>
    std::vector<std::pair<int, int>> m_cells;

    // -- Cach index computation for grid indices
    std::map<std::tuple<int, int, int>, int> m_idxCache;

    // -- Open vdb
    void WriteVdb(
        const std::vector<FluidParticle*>& particles,
        size_t frameNumber
        );
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
