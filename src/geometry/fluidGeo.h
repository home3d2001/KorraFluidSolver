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

    // Override to update transform feedback buffers
    virtual void EnableVertexAttributes() const;
    virtual void UpdateVAO();

    void UpdatePositions(const std::vector<glm::vec3>& positions);

protected:
    virtual void InitIndices();

    // -- Secondary vao to ping-pong transform feedback

    // Which vao we are using
    bool m_useVao2;

    // Vertex buffers
    GLuint m_posBuffer2;

    // Vertex array object
    GLuint m_vao2;
};

#endif
