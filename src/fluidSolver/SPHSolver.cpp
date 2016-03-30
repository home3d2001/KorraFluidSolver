#include "SPHSolver.h"

// ---------------------------------------------------- //
// SPHSolver
// ---------------------------------------------------- //
SPHSolver::SPHSolver(
    const glm::vec3& containerDim,
    const glm::vec3& particleDim,
    const float separation,
    const float cellSize,
    const float stiffness,
    const float viscosity,
    const float mass,
    const float restDensity,
    const float timestep
    ) : FluidSolver(containerDim, particleDim, separation, mass),
        m_grid(nullptr),
        m_cellSize(cellSize),
        m_stiffness(stiffness),
        m_viscosity(viscosity),
        m_restDensity(restDensity),
        m_timestep(timestep),
        m_kernelRadius(cellSize * 2.0f)
{
    m_grid = new SPHGrid(
        m_particles,
        -m_containerDim,
        m_containerDim,
        m_cellSize,
        true
    );

    srand(time(NULL));
}

SPHSolver::~SPHSolver()
{
    delete m_grid;
}

void
SPHSolver::Update(
    const float deltaT
    )
{
    m_grid->ResetGrid(m_particles);

    std::vector<FluidParticle*> neighbors;
    for (FluidParticle* p : m_particles) {
        neighbors.clear();
        neighbors = m_grid->SearchNeighbors(p);
        this->CalculateDensity(p, neighbors);
        this->CalculatePressure(p);
    }

    for (FluidParticle* p : m_particles) {
        neighbors.clear();
        neighbors = m_grid->SearchNeighbors(p);
        this->CalculatePressureForceField(p, neighbors);
        this->CalculateViscosityForceField(p, neighbors);
    }

    for (FluidParticle* p : m_particles) {
        this->UpdateDynamics(p, deltaT);
    }
}

void
SPHSolver::CalculateDensity(
    FluidParticle* particle,
    const std::vector<FluidParticle*>& neighbors
    )
{
    // -- Compute density over a kernel function of neighbors
    float density = 0.f;
    for (FluidParticle* neighbor : neighbors) {
        float tempDensity = KernelPoly6(glm::distance(neighbor->Position(), particle->Position()), m_kernelRadius);
        density += tempDensity;
    }
    density = m_mass * density;
    particle->SetDensity(density);
}

void
SPHSolver::CalculatePressure(
    FluidParticle* particle
    )
{
    float pressure = m_stiffness * (particle->Density() - m_restDensity);

    // Clamp from being negative
    pressure = pressure < 0.0 ? 0 : pressure;
    particle->SetPressure(pressure);
}

void
SPHSolver::CalculatePressureForceField(
    FluidParticle* particle,
    const std::vector<FluidParticle*>& neighbors
    )
{
    // -- Compute pressure gradient
    glm::vec3 pressureGrad(0.0f);
    for (FluidParticle* neighbor : neighbors) {
        glm::vec3 r = particle->Position() - neighbor->Position();
        float x = glm::distance(neighbor->Position(), particle->Position());
        glm::vec3 kernelGrad = GradKernelSpiky(r, x, m_kernelRadius);

        float tempPressureForce =
            (
                particle->Pressure() / (pow(particle->Density(), 2.0f)) +
                neighbor->Pressure() / (pow(neighbor->Density(), 2.0f))
            );
        pressureGrad += tempPressureForce * kernelGrad;
    }
    pressureGrad = -pressureGrad * m_mass * m_mass;
    particle->SetPressureForce(pressureGrad);
    // particle->SetColor(glm::vec4(glm::abs(pressureGrad), 1.f));
}

void
SPHSolver::CalculateViscosityForceField(
    FluidParticle* particle,
    const std::vector<FluidParticle*>& neighbors
    )
{
    glm::vec3 viscosityForce(0.0f);
    for (FluidParticle* neighbor : neighbors) {

        float laplacianKernelViscous = LaplacianKernelViscous(glm::distance(particle->Position(), neighbor->Position()), m_kernelRadius);
        glm::vec3 tempVisForce = (neighbor->Velocity() - particle->Velocity()) * laplacianKernelViscous / neighbor->Density();
        viscosityForce += tempVisForce;
    }
    viscosityForce = viscosityForce * m_mass * m_viscosity;
    particle->SetViscosityForce(viscosityForce);
}


void
SPHSolver::UpdateDynamics(
    FluidParticle* particle,
    const float deltaT
    )
{
    particle->Update(deltaT);

    // Check boundary
    glm::vec3 newPosition = particle->Position();
    glm::vec3 newVel = particle->Velocity();
    if (particle->Position().x < m_minBoundary.x) {
        newPosition.x = m_minBoundary.x + 0.0f;
        newVel.x = -newVel.x * 0.3;
    } else if (particle->Position().x > m_maxBoundary.x) {
        newPosition.x = m_maxBoundary.x - 0.0f;
        newVel.x = -newVel.x * 0.3;
    }

    if (particle->Position().y < m_minBoundary.y) {
        newPosition.y = m_minBoundary.y + 0.0f;
        newVel.y = -newVel.y * 0.3;
    } else if (particle->Position().y > m_maxBoundary.y) {
        newPosition.y = m_maxBoundary.y - 0.0f;
        newVel.y = -newVel.y * 0.3f;
    }

    if (particle->Position().z < m_minBoundary.z) {
        newPosition.z = m_minBoundary.z + 0.0f;
        newVel.z = -newVel.z * 0.3;
    } else if (particle->Position().z > m_maxBoundary.z) {
        newPosition.z = m_maxBoundary.z - 0.0f;
        newVel.z = -newVel.z * 0.3;
    }

    particle->SetPosition(newPosition);
    particle->SetVelocity(newVel);
}
