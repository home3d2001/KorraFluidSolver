#include "box.h"

void
Box::InitPositions()
{
    m_positions.clear();
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
    m_indices.clear();
    if (m_drawMode == DrawMode_Wireframe)
    {
        // Square for each face
        for(int i = 0; i < 6; i++){
            m_indices.push_back(i * 4);
            m_indices.push_back(i * 4 + 1);
            m_indices.push_back(i * 4 + 1);
            m_indices.push_back(i * 4 + 2);
            m_indices.push_back(i * 4 + 2);
            m_indices.push_back(i * 4 + 3);
            m_indices.push_back(i * 4 + 3);
            m_indices.push_back(i * 4);
        }
    }
    else if (m_drawMode == DrawMode_Shaded)
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
}

void
Box::Create()
{
    InitPositions();
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

    // @todo:
    // glGenBuffers(1, &m_norBuffer);

    // @todo:
    // glGenBuffers(1, &m_colBuffer);

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
