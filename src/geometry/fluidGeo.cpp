#include <geometry/fluidGeo.h>

FluidGeo::FluidGeo(
    const vector<glm::vec3>& positions
    ) : m_useVao2(false)
{
    UpdatePositions(positions);
}

GLenum
FluidGeo::GLDrawMode() const
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
FluidGeo::EnableVertexAttributes() const
{
    GLuint curvao = m_useVao2 ? m_vao2 : m_vao;
    GLuint curposBuffer = m_useVao2 ? m_posBuffer2 : m_posBuffer;

    glBindVertexArray(curvao);

    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, curposBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Bind element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuffer);
}

void
FluidGeo::Create()
{
    InitIndices();

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_posBuffer);
    glGenBuffers(1, &m_idxBuffer);

    // -- Secondary vao to ping-pong transform feedback
    glGenVertexArrays(1, &m_vao2);
    glGenBuffers(1, &m_posBuffer2);

    // Share the same m_idxBuffer
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m_indices.size() * sizeof(GLushort),
        &m_indices[0],
        GL_STATIC_DRAW
        );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);

    glBindVertexArray(m_vao2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuffer);
    glBindVertexArray(NULL);

    UpdateVAO();
}

void
FluidGeo::UpdateVAO()
{
    GLuint curvao = m_useVao2 ? m_vao2 : m_vao;
    GLuint curposBuffer = m_useVao2 ? m_posBuffer2 : m_posBuffer;

    glBindVertexArray(curvao);
    glBindBuffer(GL_ARRAY_BUFFER, curposBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_positions.size() * sizeof(glm::vec3),
        &m_positions[0],
        GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    glBindVertexArray(NULL);
}

// ------ Private ------ //
void
FluidGeo::InitIndices()
{
    int len = m_positions.size();
    for (int i = 0; i < len; ++i)
    {
        m_indices.push_back(i);
    }
}
