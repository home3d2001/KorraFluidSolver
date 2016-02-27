#include "box.h"

Box::Box(
    ViewMode viewMode
    )
{
    m_viewMode = viewMode;
    InitPositions();
    InitIndices();
}

void
Box::InitPositions()
{
    //Front face
    //UR
    m_positions.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
    //LR
    m_positions.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
    //LL
    m_positions.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
    //UL
    m_positions.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));

    //Right face
    //UR
    m_positions.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
    //LR
    m_positions.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
    //LL
    m_positions.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
    //UL
    m_positions.push_back(glm::vec3(0.5f, 0.5f, 0.5f));

    //Left face
    //UR
    m_positions.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
    //LR
    m_positions.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
    //LL
    m_positions.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
    //UL
    m_positions.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));

    //Back face
    //UR
    m_positions.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
    //LR
    m_positions.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
    //LL
    m_positions.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
    //UL
    m_positions.push_back(glm::vec3(0.5f, 0.5f, -0.5f));

    //Top face
    //UR
    m_positions.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
    //LR
    m_positions.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
    //LL
    m_positions.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
    //UL
    m_positions.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));

    //Bottom face
    //UR
    m_positions.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
    //LR
    m_positions.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
    //LL
    m_positions.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
    //UL
    m_positions.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
}

void
Box::InitNormals()
{

}

void
Box::InitColors()
{

}

void
Box::InitIndices()
{
    // Two triangles for each faces
    for(int i = 0; i < 6; i++){
        m_indices.push_back(i * 4);
        m_indices.push_back(i * 4 + 1);
        m_indices.push_back(i * 4 + 2);
        m_indices.push_back(i * 4);
        m_indices.push_back(i * 4 + 2);
        m_indices.push_back(i * 4 + 3);
    }
}

void
Box::Create()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // @todo: Is it faster to allocate all the buffers ahead of time?

    // // -- Position
    // glGenBuffers(1, &m_posBuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
    // glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), (void*)&(m_positions[0]), GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, NULL);

    // // @todo:
    // // glGenBuffers(1, &m_norBuffer);

    // // @todo:
    // // glGenBuffers(1, &m_colBuffer);

    // // -- Index
    // glGenBuffers(1, &m_idxBuffer);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuffer);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), (void*)&(m_indices[0]), GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

    // Draw a triangle
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBindVertexArray(0);

}
