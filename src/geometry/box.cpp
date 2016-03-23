#include "box.h"

const glm::vec3
Box::GetMinBoundary() const
{
    // @todo: use global transformation when building a scene graph
    return m_translation - m_scale / 2.0f;
}

const glm::vec3
Box::GetMaxBoundary() const
{
    // @todo: use global transformation when building a scene graph
    return m_translation + m_scale / 2.0f;
}


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
    glGenBuffers(1, &m_posBuffer);
    glGenBuffers(1, &m_idxBuffer);

    UpdateVAO();
}
