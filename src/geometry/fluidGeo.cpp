#include <geometry/fluidGeo.h>
#include <iostream>
#include <string>

FluidGeo::FluidGeo(
    const vector<glm::vec3>& positions,
    const vector<glm::vec3>& velocities,
    const vector<float>& spawnTimes,
    const vector<glm::vec4>& colors
    ) :
        m_velocities(velocities),
        m_spawnTimes(spawnTimes),
        m_useVao2(false)
{
    m_positions = positions;
    m_colors = colors;
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

GLuint
FluidGeo::ColBuffer() const
{
    return m_useVao2 ? m_colBuffer2 : m_colBuffer;
}

void
FluidGeo::EnableVertexAttributes() const
{
    GLuint curvao = m_useVao2 ? m_vao2 : m_vao;
    GLuint curposBuffer = m_useVao2 ? m_posBuffer2 : m_posBuffer;
    GLuint curvelBuffer = m_useVao2 ? m_velBuffer2 : m_velBuffer;
    GLuint curspawntimeBuffer = m_useVao2 ? m_spawnTimeBuffer2 : m_spawnTimeBuffer;
    GLuint curColBuffer = m_useVao2 ? m_colBuffer2 : m_colBuffer;

    glBindVertexArray(curvao);

    // Enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, curposBuffer);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, curvelBuffer);
    glVertexAttribPointer(VELOCITY_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, curspawntimeBuffer);
    glVertexAttribPointer(SPAWNTIME_LOCATION, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, curColBuffer);
    glVertexAttribPointer(COLOR_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(POSITION_LOCATION);
    glEnableVertexAttribArray(VELOCITY_LOCATION);
    glEnableVertexAttribArray(SPAWNTIME_LOCATION);
    glEnableVertexAttribArray(COLOR_LOCATION);
}

void
FluidGeo::DisableVertexAttributes() const
{
    glDisableVertexAttribArray(POSITION_LOCATION);
    glDisableVertexAttribArray(VELOCITY_LOCATION);
    glDisableVertexAttribArray(SPAWNTIME_LOCATION);
    glDisableVertexAttribArray(COLOR_LOCATION);
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
    glGenBuffers(1, &m_colBuffer);

    // -- Secondary vao to ping-pong transform feedback
    glGenVertexArrays(1, &m_vao2);
    glGenBuffers(1, &m_posBuffer2);
    glGenBuffers(1, &m_velBuffer2);
    glGenBuffers(1, &m_spawnTimeBuffer2);
    glGenBuffers(1, &m_colBuffer2);

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
    GLuint curColBuffer = m_useVao2 ? m_colBuffer2 : m_colBuffer;

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

    glBindBuffer(GL_ARRAY_BUFFER, curColBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_colors.size() * sizeof(glm::vec4),
        &m_colors[0],
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
