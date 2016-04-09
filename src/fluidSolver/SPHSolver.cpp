#include "SPHSolver.h"

// ---------------------------------------------------- //
// SPHSolver
// ---------------------------------------------------- //
SPHSolver::SPHSolver(
    const glm::vec3& containerDim,
    const glm::vec3& particleDim,
    const float separation,
    const double cellSize,
    const float stiffness,
    const float viscosity,
    const float mass,
    const float restDensity
    ) : FluidSolver(containerDim, particleDim, separation, mass),
        m_grid(nullptr),
        m_stiffness(stiffness),
        m_viscosity(viscosity),
        m_restDensity(restDensity),
        m_kernelRadius(cellSize * 2.0)
{
    m_grid = new SPHGrid(
        m_particles,
        -m_containerDim,
        m_containerDim,
        cellSize,
        true
    );

    srand(time(NULL));
}

SPHSolver::~SPHSolver()
{
    delete m_grid;
}

void
SPHSolver::SetConstant(
    SPHConstantType type,
    float value
    )
{
    switch(type) {
        case SPHStiffness:
            m_stiffness = value;
            break;
        case SPHViscosity:
            m_viscosity = value;
            break;
        case SPHRestDensity:
            m_restDensity = value;
            break;
        case SPHMass:
            FluidParticle::mass = value;
            break;
    }
}

void
SPHSolver::CheckBoxIntersection(
    Box* box
    )
{
    for (FluidParticle* particle : m_particles) {
        glm::vec3 newVel;
        bool intersected = false;
        glm::vec3 newPos = box->Intersect(particle->Position(), newVel, intersected);
        if (intersected) {
            particle->SetPosition(newPos);
            particle->SetVelocity(particle->Velocity() + 2.0f * newVel);
        }
    }
}

void
SPHSolver::Update(
    const float deltaT
    )
{
    m_grid->ResetGrid(m_particles);

#define USE_TBB
#ifdef USE_TBB
    // -- Search all neighbors
    int len = m_particles.size();
    parallel_for(0, len, [&](int i) {
        FluidParticle* p = m_particles[i];
        m_grid->UpdateNeighbors(p);
    });

    // -- Calculate density & pressures
    parallel_for(0, len, [&](int i) {
        FluidParticle* p = m_particles[i];
        this->CalculateDensity(p);
        this->CalculatePressure(p);
    });

    // -- Calculate force field
    parallel_for(0, len, [&](int i) {
        FluidParticle* p = m_particles[i];
        this->CalculatePressureForceField(p);
        this->CalculateViscosityForceField(p);
    });

    // -- Update particles
    parallel_for(0, len, [&](int i) {
        FluidParticle* p = m_particles[i];
        this->UpdateDynamics(p, deltaT);
    });

#else

    // -- Search all neighbors
    for (FluidParticle* p : m_particles) {
        m_grid->UpdateNeighbors(p);
    }

    // -- Calculate density & pressures
    for (FluidParticle* p : m_particles) {
        this->CalculateDensity(p);
        this->CalculatePressure(p);
    }

    // -- Calculate force field
    for (FluidParticle* p : m_particles) {
        this->CalculatePressureForceField(p);
        this->CalculateViscosityForceField(p);
    }

    // -- Update particles
    for (FluidParticle* p : m_particles) {
        this->UpdateDynamics(p, deltaT);
    }

#endif
}

void
SPHSolver::CalculateDensity(
    FluidParticle* particle
    )
{
    // -- Compute density over a kernel function of neighbors
    float density = 0.f;
    for (size_t i = 0; i < particle->neighborsCount; ++i) {
        FluidParticle* neighbor = particle->neighbors[i];
        float tempDensity = KernelPoly6(glm::distance(neighbor->Position(), particle->Position()), m_kernelRadius);
        density += tempDensity;
    }
    density = FluidParticle::mass * density;
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
    FluidParticle* particle
    )
{
    // -- Compute pressure gradient
    glm::vec3 pressureGrad(0.0f);
    for (size_t i = 0; i < particle->neighborsCount; ++i) {
        FluidParticle* neighbor = particle->neighbors[i];
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
    pressureGrad = -pressureGrad * FluidParticle::mass * FluidParticle::mass;
    particle->SetPressureForce(pressureGrad);
}

void
SPHSolver::CalculateViscosityForceField(
    FluidParticle* particle
    )
{
    glm::vec3 viscosityForce(0.0f);
    for (size_t i = 0; i < particle->neighborsCount; ++i) {
        FluidParticle* neighbor = particle->neighbors[i];
        float laplacianKernelViscous = LaplacianKernelViscous(glm::distance(particle->Position(), neighbor->Position()), m_kernelRadius);
        glm::vec3 tempVisForce = (neighbor->Velocity() - particle->Velocity()) * laplacianKernelViscous / neighbor->Density();
        viscosityForce += tempVisForce;
    }
    viscosityForce = viscosityForce * FluidParticle::mass * m_viscosity;
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
        newVel.x = -newVel.x * 0.5;
        newVel.y *= 0.9;
        newVel.z *= 0.9;
    } else if (particle->Position().x > m_maxBoundary.x) {
        newPosition.x = m_maxBoundary.x - 0.0f;
        newVel.x = -newVel.x * 0.5;
        newVel.y *= 0.9;
        newVel.z *= 0.9;
    }

    if (particle->Position().y < m_minBoundary.y) {
        newPosition.y = m_minBoundary.y + 0.0f;
        newVel.y = -newVel.y * 0.5;
        newVel.x *= 0.9;
        newVel.z *= 0.9;
    } else if (particle->Position().y > m_maxBoundary.y) {
        newPosition.y = m_maxBoundary.y - 0.0f;
        newVel.y = -newVel.y * 0.5f;
        newVel.x *= 0.9;
        newVel.z *= 0.9;
    }

    if (particle->Position().z < m_minBoundary.z) {
        newPosition.z = m_minBoundary.z + 0.0f;
        newVel.z = -newVel.z * 0.5;
        newVel.x *= 0.9;
        newVel.y *= 0.9;
    } else if (particle->Position().z > m_maxBoundary.z) {
        newPosition.z = m_maxBoundary.z - 0.0f;
        newVel.z = -newVel.z * 0.5;
        newVel.x *= 0.9;
        newVel.y *= 0.9;
    }

    particle->SetPosition(newPosition);
    particle->SetVelocity(newVel);
}
