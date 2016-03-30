#include "FluidParticle.h"

// ---------------------------------------------------- //
// FluidParticle
// ---------------------------------------------------- //
FluidParticle::FluidParticle()
{

}

FluidParticle::FluidParticle(
    const glm::vec3& pos,
    const glm::vec3& vel,
    const glm::vec4& col
    ) :m_pos(pos), m_vel(vel), m_col(col)
{

}

void
FluidParticle::Update(
    const float deltaT
    )
{
    // @todo: HACK HERE!
    float mass = 0.125;
    float cellSize = 0.10001;

    glm::vec3 gravity(0.0f, mass * GRAVITY, 0.0f);
    glm::vec3 totalForce = m_accel + m_pressureForce + m_viscosityForce + gravity;
    m_vel = m_vel + totalForce * deltaT;

    // -- Enforce CFL condition
    float vMax = 0.4 * cellSize / deltaT;
    if (glm::length(m_vel) > vMax) {
        m_vel = glm::normalize(m_vel) * vMax;
    }
    m_pos = m_pos + m_vel * deltaT;
}
