#include <geometry/fluidGeo.h>
#include <iostream>
#include <string>

FluidGeo::FluidGeo(
    const vector<glm::vec3>& positions,
    const vector<glm::vec3>& velocities,
    const vector<float>& spawnTimes
    ) :
        m_velocities(velocities),
        m_spawnTimes(spawnTimes),
        m_useVao2(false)
{
    m_positions = positions;
}

GLenum
FluidGeo::GLDrawMode() const
{
    return GL_POINTS;
}

void
FluidGeo::ToggleVao()
{
    m_useVao2 = !m_useVao2;
}

GLuint
FluidGeo::PosBuffer() const
{
    return m_useVao2 ? m_posBuffer2 : m_posBuffer;
}

GLuint
FluidGeo::VelBuffer() const
{
    return m_useVao2 ? m_velBuffer2 : m_velBuffer;
}

GLuint
FluidGeo::SpawnTimeBuffer() const
{
    return m_useVao2 ? m_spawnTimeBuffer2 : m_spawnTimeBuffer;
}

void
FluidGeo::EnableVertexAttributes() const
{
    GLuint curvao = m_useVao2 ? m_vao2 : m_vao;
    GLuint curposBuffer = m_useVao2 ? m_posBuffer2 : m_posBuffer;
    GLuint curvelBuffer = m_useVao2 ? m_velBuffer2 : m_velBuffer;
    GLuint curspawntimeBuffer = m_useVao2 ? m_spawnTimeBuffer2 : m_spawnTimeBuffer;

    glBindVertexArray(curvao);

    // Enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, curposBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, curvelBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, curspawntimeBuffer);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void
FluidGeo::DisableVertexAttributes() const
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);
}

void
FluidGeo::Create()
{
    InitIndices();

    glGenBuffers(1, &m_idxBuffer);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_posBuffer);
    glGenBuffers(1, &m_velBuffer);
    glGenBuffers(1, &m_spawnTimeBuffer);

    // -- Secondary vao to ping-pong transform feedback
    glGenVertexArrays(1, &m_vao2);
    glGenBuffers(1, &m_posBuffer2);
    glGenBuffers(1, &m_velBuffer2);
    glGenBuffers(1, &m_spawnTimeBuffer2);

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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);

    UpdateVao();
    ToggleVao();
    UpdateVao();
    ToggleVao();
}

void
FluidGeo::UpdateVao()
{
    GLuint curvao = m_useVao2 ? m_vao2 : m_vao;
    GLuint curposBuffer = m_useVao2 ? m_posBuffer2 : m_posBuffer;
    GLuint curvelBuffer = m_useVao2 ? m_velBuffer2 : m_velBuffer;
    GLuint curspawntimeBuffer = m_useVao2 ? m_spawnTimeBuffer2 : m_spawnTimeBuffer;

    glBindVertexArray(curvao);
    glBindBuffer(GL_ARRAY_BUFFER, curposBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_positions.size() * sizeof(glm::vec3),
        &m_positions[0],
        GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, curvelBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_velocities.size() * sizeof(glm::vec3),
        &m_velocities[0],
        GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, curspawntimeBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_spawnTimes.size() * sizeof(float),
        &m_spawnTimes[0],
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
