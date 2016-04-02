#ifndef SCENE_H
#define SCENE_H

#include <shaderProgram/shaderProgram.h>
#include <shaderProgram/particleAdvectProgram.h>
#include <geometry/box.h>
#include <geometry/fluidGeo.h>
#include <fluidSolver/SPHSolver.h>
#include <input/keyboardControl.h>

class Scene
{
public:
    Scene();
    Scene(
        const float& screenWidth,
        const float& screenHeight
    );

    void InitFromTestScene();
    void InitFromJson(const char* filepath);
    void Pause();
    void SetConstant(
        SPHConstantType type,
        float value
        );

    virtual void ReadInputs(KeyCode key, KeyAction action);
    virtual void Update(
        const float deltaT,
        ParticleAdvectProgram& prog
        );
    virtual void Draw(const ShaderProgram&) const;
    virtual void DrawTransformFeedback(
        const ShaderProgram& prog,
        ParticleAdvectProgram& progAdvect
        );
    virtual void CleanUp();

    inline const uint NumParticles() const {
        if (m_fluidSolver) {
            return m_fluidSolver->NumParticles();
        } else {
            return 0;
        }
    }

protected:
    bool m_paused = false;
    Camera* m_camera;
    Box* m_testBox;
    Box* m_fluidContainer;
    FluidGeo* m_fluidGeo;
    SPHSolver* m_fluidSolver;

    // -- Update helpers
    void UpdateCamera(KeyCode key);
    void UpdateFluidSolver(
        const float deltaT,
        ParticleAdvectProgram& prog
        );

    // -- Draw helpers
    void DrawFluidSolver(
        const ShaderProgram& prog,
        ParticleAdvectProgram& progAdvect
        );
};

#endif
