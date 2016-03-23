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
        std::vector<FluidParticle*> particles,
        int width,
        int height,
        int depth,
        float cellSize
        );
    void AddParticle(FluidParticle* particle);
    std::vector<FluidParticle*> GetParticles(int i, int j, int k);
    std::vector<FluidParticle*> GetNeighborParticles(int i, int j, int k);

private:

    int m_width;  // i
    int m_height; // j
    int m_depth;  // k
    int m_cellSize;

    // -- cells
    std::vector<std::vector<FluidParticle*>> cells;

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
