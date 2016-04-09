#include "box.h"

Box::Box(
    DrawMode drawMode
    )
{
    SetDrawMode(drawMode);
    InitPositions();
    InitNormals();
    InitColors();
    InitIndices();
}

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

glm::vec3
Box::Intersect(
    const glm::vec3& position,
    glm::vec3& newVelocity,
    bool& intersected
    )
{
    glm::vec3 localPos = glm::vec3(glm::inverse(m_localTransform) * glm::vec4(position, 1.0f));
    if (localPos.x < 0.5f && localPos.x > -0.5f &&
        localPos.y < 0.5f && localPos.y > -0.5f &&
        localPos.z < 0.5f && localPos.z > -0.5f
        )
    {
        // Find the nearest face, the push it out that way
        float minDis = 99999;
        float dis[6] = {
            fabs(localPos.x - 0.5f),
            fabs(localPos.x + 0.5f),
            fabs(localPos.y - 0.5f),
            fabs(localPos.y + 0.5f),
            fabs(localPos.z - 0.5f),
            fabs(localPos.z + 0.5f)
        };

        BoxFace face = BoxFace_PosX;
        for (char f = 0; f < (char)BoxFace_Max; ++f) {
            if (dis[f] < minDis) {
                minDis = dis[f];
                face = (BoxFace)f;
            }
        }

        switch(face) {
            case BoxFace_PosX:
                localPos.x = 0.6f;
                newVelocity = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
            case BoxFace_NegX:
                localPos.x = -0.6f;
                newVelocity = glm::vec3(-1.0f, 0.0f, 0.0f);
                break;
            case BoxFace_PosY:
                localPos.y = 0.6f;
                newVelocity = glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            case BoxFace_NegY:
                localPos.y = -0.6f;
                newVelocity = glm::vec3(0.0f, -1.0f, 0.0f);
                break;
            case BoxFace_PosZ:
                localPos.z = 0.6f;
                newVelocity = glm::vec3(0.0f, 0.0f, 1.0f);
                break;
            case BoxFace_NegZ:
                localPos.z = -0.6f;
                newVelocity = glm::vec3(0.0f, 0.0f, -1.0f);
                break;
        }
        newVelocity = glm::vec3(m_localTransform * glm::vec4(newVelocity, 0.0f));

        intersected = true;

        glm::vec3 positionWorld = glm::vec3(m_localTransform * glm::vec4(localPos, 1.0f));
        return positionWorld;
    }

    return position;
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
    m_normals.clear();
    // Front face
    m_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    //LR
    m_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    //LL
    m_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    //UL
    m_normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

    //Right face
    //UR
    m_normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    //LR
    m_normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    //LL
    m_normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    //UL
    m_normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

    //Left face
    //UR
    m_normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    //LR
    m_normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    //LL
    m_normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    //UL
    m_normals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));

    //Back face
    //UR
    m_normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    //LR
    m_normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    //LL
    m_normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
    //UL
    m_normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

    //Top face
    //UR
    m_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    //LR
    m_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    //LL
    m_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    //UL
    m_normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

    //Bottom face
    //UR
    m_normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    //LR
    m_normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    //LL
    m_normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    //UL
    m_normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
}

void
Box::InitColors()
{
    if (m_colors.size() == 0) {
        // Set default color
        int len = m_positions.size();
        for (int i = 0; i < len; ++i) {
            m_colors.push_back(glm::vec4(0, 1.0f, 1.0f, 1.0f));
        }
    }
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
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_posBuffer);
    glGenBuffers(1, &m_norBuffer);
    glGenBuffers(1, &m_colBuffer);
    glGenBuffers(1, &m_idxBuffer);

    UpdateVAO();
}
