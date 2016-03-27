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
	glm::vec3 gravity(0.0f, GRAVITY, 0.0f);
	m_vel = m_vel + (m_accel + gravity) * deltaT;
	m_pos = m_pos + m_vel * deltaT * 0.1f;
}
