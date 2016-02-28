#ifndef SCENE_H
#define SCENE_H

#include <geometry/box.h>
#include <geometry/fluidGeo.h>
#include <fluidSolver/fluidSolver.h>

class Scene
{
public:
    Scene();
    ~Scene();
    void InitFromTestScene();
    void InitFromJson(const char* filepath);

    virtual void Update();
    virtual const Geometry* RootGeometry() const;

protected:
    Box* m_testBox;
    Box* m_fluidContainer;
    FluidGeo* m_fluidGeo;
    FluidSolver* m_fluidSolver;
};

#endif
