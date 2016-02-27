#ifndef GEOMETRY_H
#define GEOMETRY_H

// GL libraries

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// C++

#include <vector>

using namespace std;

typedef enum {
    WireFrame,
    Shaded
} ViewMode;

class Geometry
{
public:
    Geometry () { m_viewMode = Shaded; };
    virtual void Create() = 0;

    // -- OpenGL helpers
    virtual GLenum DrawMode() const;
    virtual GLsizei ElementCount() const;

    // Bind the vertex array object for this geometry
    virtual void EnableVertexAttributes() const;
    virtual void DisableVertexAttributes() const;

    // -- Transformations
    virtual void SetLocalTransformation(const glm::mat4);
    virtual void SetGlobalTransformation(const glm::mat4);
    virtual glm::mat4 GetLocalTransformation() const;
    virtual glm::mat4 GetGlobalTransformation() const;

protected:
    ViewMode m_viewMode;

    // Data
    vector<glm::vec3> m_positions;
    vector<glm::vec3> m_normals;
    vector<glm::vec3> m_colors;
    vector<GLushort> m_indices;

    // Transformation
    glm::mat4 m_localTransform;
    glm::mat4 m_globalTransform;

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
