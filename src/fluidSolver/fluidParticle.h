#ifndef FLUID_PARTICLE_H
#define FLUID_PARTICLE_H

#include <glm/glm.hpp>

// ---------------------------------------------------- //
// FluidParticle
// ---------------------------------------------------- //
class FluidParticle
{
public:
    FluidParticle();
    FluidParticle(
        const glm::vec3& pos,
        const glm::vec3& vel,
        const glm::vec4& col
        );

    // Getters/setters
    const glm::vec3& Position() const { return m_pos; }
    const glm::vec3& Velocity() const { return m_vel; }
    const glm::vec4& Color() const { return m_col; }
    void SetColor(const glm::vec4& color) { m_col = color; }
    const float& Density() const { return m_density; }
    const float& Pressure() const { return m_pressure; }
    const float& SpawnTime() const { return m_spawnTime; }

protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;
    glm::vec4 m_col;
    float m_density = 1.0f;
    float m_pressure = 1.0f;
    float m_spawnTime = 0.0f;
};

#endif
