#include <geometry/fluidGeo.h>

FluidGeo::FluidGeo(
    const vector<glm::vec3>& positions
    )
{
    UpdatePositions(positions);
}

GLenum
FluidGeo::GLDrawMode()
{
    return GL_POINTS;
}

void
FluidGeo::UpdatePositions(
    const std::vector<glm::vec3>& positions
    )
{
    m_positions = positions;
}

void
FluidGeo::InitIndices()
{
    int len = m_positions.size();
    for (int i = 0; i < len; ++i)
    {
        m_indices.push_back(i);
    }
}

void
FluidGeo::Create()
{
    InitIndices();

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // @todo: Is it faster to allocate all the buffers ahead of time?

    // -- Position

    glGenBuffers(1, &m_posBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_positions.size() * sizeof(glm::vec3),
        &m_positions[0],
        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    // -- Index

    glGenBuffers(1, &m_idxBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m_indices.size() * sizeof(GLushort),
        &m_indices[0],
        GL_STATIC_DRAW
        );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

    glBindVertexArray(NULL);
}
