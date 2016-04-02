#ifndef FLUID_PARTICLE_H
#define FLUID_PARTICLE_H

#include <iostream>

#include <glm/glm.hpp>
#include <mathConstants.h>

using namespace std;

#define NEIGHBORS_MAX 1000

typedef enum {
    SPHColorSimple,
    SPHColorPressure,
    SPHColorViscosity,
    SPHColorVelocity,
    SPHColorAllForces
} SPHColor;

// ---------------------------------------------------- //
// FluidParticle
// ---------------------------------------------------- //
class FluidParticle
{
public:

    static float mass;
    static SPHColor colorType;

    FluidParticle();
    FluidParticle(
        const glm::vec3& pos,
        const glm::vec3& vel,
        const glm::vec4& col
        );

    virtual void Update(const float deltaT);

    // -- Getters/setters
    inline const glm::vec3& Position() const { return m_pos; }
    inline void SetPosition(const glm::vec3& pos) { m_pos = pos; }
    inline const glm::vec3& Velocity() const { return m_vel; }
    inline void SetVelocity(const glm::vec3& vel) { m_vel = vel; }
    inline const glm::vec4& Color() const {
        switch(FluidParticle::colorType) {
            case SPHColorSimple:
                return m_col;
            case SPHColorPressure:
                return glm::vec4(glm::abs(m_pressureForce), 1.0f);
            case SPHColorViscosity:
                return glm::vec4(glm::abs(m_viscosityForce), 1.0f);
            case SPHColorVelocity:
                return glm::vec4(glm::abs(m_vel), 1.0f);
            case SPHColorAllForces:
                return glm::vec4(glm::abs(m_force), 1.0f);
            default:
                return m_col;
        }
    }
    inline void SetColor(const glm::vec4& color) { m_col = color; }
    inline const float& Density() const { return m_density; }
    inline void SetDensity(float density) { m_density = density; }
    inline const float Pressure() const { return m_pressure; }
    inline void SetPressure(const float pressure) { m_pressure = pressure; }
    inline const glm::vec3& PressureForce() const { return m_pressureForce; }
    inline void SetPressureForce(const glm::vec3& pressureForce) { m_pressureForce = pressureForce; }
    inline const glm::vec3& ViscosityForce() const { return m_viscosityForce; }
    inline void SetViscosityForce(const glm::vec3& viscosityForce) { m_viscosityForce = viscosityForce; }
    inline const float& SpawnTime() const { return m_spawnTime; }
    inline void SetSpawnTime(float spawnTime) { m_spawnTime = spawnTime; }
    inline void ResetNeighborCount() { neighborsCount = 0; }

    // @return true if successful
    inline bool AddNeighbor(FluidParticle* neighbor) {
        if (neighborsCount < NEIGHBORS_MAX) {
            neighbors[neighborsCount++] = neighbor;
            return true;
        } else {
            return false;
        }
    }

    FluidParticle* neighbors[NEIGHBORS_MAX];
    size_t neighborsCount = 0;

protected:
    glm::vec3 m_pos;
    glm::vec3 m_vel;
    glm::vec4 m_col;
    glm::vec3 m_pressureForce;
    glm::vec3 m_viscosityForce;
    glm::vec3 m_force;
    float m_pressure = 0.0f;
    float m_density = 1.0f;
    float m_spawnTime = 0.0f;
};

#endif
