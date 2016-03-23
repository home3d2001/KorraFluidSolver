#ifndef SPHSOLVER_H
#define SPHSOLVER_H

#include <fluidSolver/fluidSolver.h>
#include <fluidSolver/SPHGrid.h>

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
