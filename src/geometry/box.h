#ifndef BOX_H
#define BOX_H

#include <geometry/geometry.h>

typedef enum {
    BoxFace_PosX,
    BoxFace_NegX,
    BoxFace_PosY,
    BoxFace_NegY,
    BoxFace_PosZ,
    BoxFace_NegZ,
    BoxFace_Max
} BoxFace;

class Box : public Geometry
{
public:
    virtual void Create();
    virtual glm::vec3 Intersect(
        const glm::vec3& position,
        glm::vec3& newVelocity,
        bool& intersected
        );
    virtual const glm::vec3 GetMinBoundary() const;
    virtual const glm::vec3 GetMaxBoundary() const;

protected:
    virtual void InitPositions();
    virtual void InitNormals();
    virtual void InitColors();
    virtual void InitIndices();
};

#endif
