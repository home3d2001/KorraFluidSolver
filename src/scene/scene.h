#ifndef SCENE_H
#define SCENE_H

#include <shaderProgram/shaderProgram.h>
#include <geometry/box.h>
#include <geometry/fluidGeo.h>
#include <fluidSolver/fluidSolver.h>
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

    virtual void Update(const KeyboardControl*);
    virtual void Draw(const ShaderProgram&) const;
    virtual void DrawTransformFeedback(
        const ShaderProgram& progUpdate,
        const ShaderProgram& progDraw
        ) const;
    virtual void CleanUp();

protected:
    Camera* m_camera;
    Box* m_testBox;
    Box* m_fluidContainer;
    FluidGeo* m_fluidGeo;
    FluidSolver* m_fluidSolver;

    // -- Update helpers
    void UpdateCamera(const KeyboardControl*);
    void UpdateFluidSolver();

    // -- Draw helpers
    void DrawFluidSolver(
        const ShaderProgram& progUpdate,
        const ShaderProgram& progDraw
        ) const;

};

#endif
