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

    virtual void Update(
        const float deltaT,
        const KeyboardControl*,
        ParticleAdvectProgram& prog
        );
    virtual void Draw(const ShaderProgram&) const;
    virtual void DrawTransformFeedback(
        const ShaderProgram& prog,
        ParticleAdvectProgram& progAdvect
        );
    virtual void CleanUp();

protected:
    Camera* m_camera;
    Box* m_testBox;
    Box* m_fluidContainer;
    FluidGeo* m_fluidGeo;
    SPHSolver* m_fluidSolver;

    // -- Update helpers
    void UpdateCamera(
        const float deltaT,
        const KeyboardControl*);
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
