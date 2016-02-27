#ifndef SCENE_H
#define SCENE_H

#include "../geometry/box.h"

class Scene
{
public:
    Scene();
    ~Scene();
    virtual void InitFromTestScene();
    virtual void InitFromJson();
    virtual const Geometry& RootGeometry() const;

protected:
    Box m_box;
};

#endif
