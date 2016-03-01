#ifndef BOX_H
#define BOX_H

#include <geometry/geometry.h>

class Box : public Geometry
{
public:
    virtual void Create();
    virtual const glm::vec3 GetMinBoundary() const;
    virtual const glm::vec3 GetMaxBoundary() const;

protected:
    virtual void InitPositions();
    virtual void InitNormals();
    virtual void InitColors();
    virtual void InitIndices();
};

#endif
