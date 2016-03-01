#ifndef GEOMETRY_H
#define GEOMETRY_H

// GL libraries

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// C++

#include <vector>

// Custom

#include <mathConstants.h>

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

    // -- Color
    void SetColor(const glm::vec4& color) {
        m_color = color;
    };
    const glm::vec4& GetColor() const {
        return m_color;
    };

    // -- Transformations
    void Translate(const float& x, const float& y, const float& z);
    void Rotate(const float& radx, const float& rady, const float& radz);
    void Scale(const float& scaleX, const float& scaleY, const float& scaleZ);
    glm::mat4 GetLocalTransformation() const;
    glm::mat4 GetGlobalTransformation() const;

protected:
    DrawMode m_drawMode = DrawMode_Shaded;

    // Data
    glm::vec4 m_color = glm::vec4(1.0);

    vector<glm::vec3> m_positions;
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
