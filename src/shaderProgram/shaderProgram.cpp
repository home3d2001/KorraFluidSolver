#include "shaderProgram.h"

ShaderProgram::ShaderProgram(
        const char* vertFilePath,
        const char* fragFilePath
        )
{
    m_programID = LoadShaders(vertFilePath, fragFilePath);

    m_unifModel = glGetUniformLocation(m_programID, "u_model");
    m_unifViewProj = glGetUniformLocation(m_programID, "u_viewProj");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void
ShaderProgram::Draw(
    const Camera& camera,
    const Geometry& geo
    ) const
{
    glUseProgram(m_programID);

    // Enable attributes
    geo.EnableVertexAttributes();

    // Set uniforms

    if (m_unifModel != -1) {
        glUniformMatrix4fv(
            m_unifModel,
            1,
            GL_FALSE,
            &(geo.GetLocalTransformation()[0][0])
            );
    }

    if (m_unifViewProj != -1) {
        glUniformMatrix4fv(
            m_unifViewProj,
            1,
            GL_FALSE,
            &(camera.GetViewProj()[0][0])
            );
    }
    // Render
    glDrawElements(
        geo.GLDrawMode(),
        geo.ElementCount(),
        GL_UNSIGNED_SHORT,
        NULL
        );

    geo.DisableVertexAttributes();
}

void
ShaderProgram::CleanUp()
{
    glDeleteProgram(m_programID);
}
