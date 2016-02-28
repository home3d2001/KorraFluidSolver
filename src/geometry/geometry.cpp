#include "geometry.h"

void
Geometry::SetDrawMode(
    DrawMode drawMode
    )
{
    m_drawMode = drawMode;
}

GLenum
Geometry::GLDrawMode() const
{
    switch(m_drawMode) {
        case DrawMode_Wireframe:
            return GL_LINES;
        case DrawMode_Vertex:
            return GL_POINTS;
        case DrawMode_Shaded:
        default:
            return GL_TRIANGLES;
    }
}

GLsizei
Geometry::ElementCount() const
{
    return m_indices.size();
}

void
Geometry::EnableVertexAttributes() const
{
    glBindVertexArray(m_vao);

    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Bind element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuffer);
}

void
Geometry::DisableVertexAttributes() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);
}

void
Geometry::Translate(
    const float& x,
    const float& y,
    const float& z
    )
{
    m_localTransform = glm::translate(m_localTransform, glm::vec3(x, y, z));
}

void
Geometry::Rotate(
    const float& radx,
    const float& rady,
    const float& radz
    )
{
    m_localTransform = glm::rotate(m_localTransform, radx, glm::vec3(1.0f, 0.0f, 0.0f));
    m_localTransform = glm::rotate(m_localTransform, rady, glm::vec3(0.0f, 1.0f, 0.0f));
    m_localTransform = glm::rotate(m_localTransform, radz, glm::vec3(0.0f, 0.0f, 1.0f));
}

void
Geometry::Scale(
    const float& scalex,
    const float& scaley,
    const float& scalez
    )
{
    m_localTransform = glm::scale(m_localTransform, glm::vec3(scalex, scaley, scalez));
}

glm::mat4
Geometry::GetLocalTransformation() const
{
    return m_localTransform;
}

glm::mat4
Geometry::GetGlobalTransformation() const
{
    return m_globalTransform;
}

void
Geometry::UpdateVAO()
{
    glBindVertexArray(m_vao);

    // @todo: Is it faster to allocate all the buffers ahead of time?

    // -- Position

    glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_positions.size() * sizeof(glm::vec3),
        &m_positions[0],
        GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    // -- Index

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
