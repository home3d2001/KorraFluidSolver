#ifndef BOX_H
#define BOX_H

#include "geometry.h"

class Box : public Geometry
{
public:
    Box(ViewMode viewMode = Shaded);
    virtual ~Box() {};
    virtual void Create();

protected:
    virtual void InitPositions();
    virtual void InitNormals();
    virtual void InitColors();
    virtual void InitIndices();
};


#endif
