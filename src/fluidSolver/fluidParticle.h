#ifndef FLUID_PARTICLE_H
#define FLUID_PARTICLE_H

#include <iostream>

#include <glm/glm.hpp>
#include <mathConstants.h>

using namespace std;

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

    virtual void Update(const float deltaT);

    // -- Getters/setters
    const glm::vec3& Position() const { return m_pos; }
    void SetPosition(const glm::vec3& pos) { m_pos = pos; }
    const glm::vec3& Velocity() const { return m_vel; }
    void SetVelocity(const glm::vec3& vel) { m_vel = vel; }
    const glm::vec3& Acceleration() const { return m_accel; }
    void SetForce(const glm::vec3& force) { m_accel = force / 0.125f; } // Mass is 1
    const glm::vec4& Color() const { return m_col; }
    void SetColor(const glm::vec4& color) { m_col = color; }
    const float& Density() const { return m_density; }
    void SetDensity(float density) { m_density = density; }
    const float Pressure() const { return m_pressure; }
    void SetPressure(const float pressure) { m_pressure = pressure; }
    const glm::vec3& PressureForce() const { return m_pressureForce; }
    void SetPressureForce(const glm::vec3& pressureForce) { m_pressureForce = pressureForce; }
    const float& SpawnTime() const { return m_spawnTime; }
    void SetSpawnTime(float spawnTime) { m_spawnTime = spawnTime; }

protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;
    glm::vec3 m_accel;
    glm::vec4 m_col;
    glm::vec3 m_pressureForce;
    float m_pressure = 0.0f;
    float m_density = 1.0f;
    float m_spawnTime = 0.0f;
};

#endif
