#ifndef SCENE_H
#define SCENE_H

#include <geometry/box.h>
#include <fluidSolver/fluidSolver.h>

class Scene
{
public:
    Scene();
    ~Scene();
    void InitFromTestScene();
    bool InitFromJson();

    virtual void Update();
    virtual const Geometry& RootGeometry() const;

protected:
    Box m_box;
};

#endif
