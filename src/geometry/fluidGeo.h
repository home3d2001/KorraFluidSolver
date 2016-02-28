#ifndef FLUID_GEO_H
#define GLUID_GEO_H

#include <geometry/geometry.h>
#include <vector>

class FluidGeo : public Geometry
{
public:
    FluidGeo(const vector<glm::vec3>& positions);

    virtual void Create();
    virtual GLenum GLDrawMode() const;

    void UpdatePositions(const std::vector<glm::vec3>& positions);

protected:
    virtual void InitIndices();
};

#endif
