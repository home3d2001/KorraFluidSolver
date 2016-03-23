#ifndef SPHSOLVER_H
#define SPHSOLVER_H

#include <fluidSolver/fluidSolver.h>

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


// ---------------------------------------------------- //
// SPHSolver
// ---------------------------------------------------- //
class SPHSolver : public FluidSolver
{
public:
    SPHSolver(
        const glm::vec3& containerDim,
        const glm::vec3& particleDim,
        const float& separation
        );

    virtual ~SPHSolver();

    virtual void Update();
    void AddParticle(FluidParticle*);

protected:

    SPHGrid* m_grid;

    // -- Particle info
    float m_stiffness;
    float m_viscosity;
    float m_radius;
    float m_mas;
    float m_restDensity;
    float m_timeStep;

    void SearchNeighbors();
    void CalculateDensity();
    void CalculatePressure();
};

#endif /* SPHSOLVER_H */
