#ifndef GEOMETRY_H
#define GEOMETRY_H

// GL libraries

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
// C++

#include <vector>
#include <iostream>

// Custom

#include <mathConstants.h>


#define POSITION_LOCATION 0
#define VELOCITY_LOCATION 1
#define SPAWNTIME_LOCATION 2
#define COLOR_LOCATION 3


typedef enum {
    DrawMode_Wireframe,
    DrawMode_Shaded,
    DrawMode_Vertex
} DrawMode;

using namespace std;

class Geometry
{
public:
    virtual void Create() = 0;
    virtual void UpdateVAO();

    // -- OpenGL helpers
    virtual void SetDrawMode(DrawMode);
    virtual GLenum GLDrawMode() const;
    virtual GLsizei ElementCount() const;

    // Bind the vertex array object for this geometry
    virtual void EnableVertexAttributes() const;
    virtual void DisableVertexAttributes() const;

    // Getters/Setters
    void SetColor(const glm::vec4& color) {
        int len = m_colors.size();
        for (int i = 0; i < len; ++i) {
            m_colors[i] = color;
        }
    }
    void SetColors(const vector<glm::vec4>& colors) {
        m_colors = colors;
    };

    void SetPositions(const std::vector<glm::vec3>& positions) {
        m_positions = positions;
    }

    virtual glm::vec3 Intersect(const glm::vec3& position) {return position;}

    // -- Transformations
    void Translate(const float& x, const float& y, const float& z);
    void Rotate(const float& radx, const float& rady, const float& radz);
    void Scale(const float& scaleX, const float& scaleY, const float& scaleZ);
    glm::mat4 GetLocalTransformation() const;
    glm::mat4 GetGlobalTransformation() const;

protected:
    DrawMode m_drawMode = DrawMode_Shaded;

    vector<glm::vec3> m_positions;
    vector<glm::vec4> m_colors;
    vector<GLushort> m_indices;

    // Transformation
    glm::mat4 m_localTransform;
    glm::mat4 m_globalTransform;
    glm::vec3 m_translation;
    glm::quat m_orientation;
    glm::vec3 m_scale;

    // Vertex buffers
    GLuint m_posBuffer;
    GLuint m_norBuffer;
    GLuint m_colBuffer;

    // Index buffer
    GLuint m_idxBuffer;

    // Vertex array object
    GLuint m_vao;
};

#endif
