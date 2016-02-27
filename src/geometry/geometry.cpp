#include "geometry.h"

GLenum
Geometry::DrawMode() const
{
    switch (m_viewMode) {
        case WireFrame:
            return GL_LINES;
        case Shaded:
        default:
            return GL_TRIANGLES;
    }
}

int
Geometry::ElementCount() const
{
    return m_indices.size();
}

void
Geometry::Bind() const
{
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void
Geometry::SetLocalTransformation(
    const glm::mat4 localTrans
    )
{

}

void
Geometry::SetGlobalTransformation(
    const glm::mat4
    )
{

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

