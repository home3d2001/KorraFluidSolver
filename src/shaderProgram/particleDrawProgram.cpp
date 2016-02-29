#include <shaderProgram/particleDrawProgram.h>

ParticleDrawProgram::ParticleDrawProgram(
    const char* vertFilePath,
    const char* fragFilePath
) : ShaderProgram(vertFilePath, fragFilePath)
{
    // Get uniform locations
    m_unifTime = glGetUniformLocation(m_programID, "u_time");
    m_unifColor = glGetUniformLocation(m_programID, "u_color");
}

void
ParticleDrawProgram::Draw(
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

    // @todo: maybe we only need to pass delta time through
    static float lastTime = glfwGetTime();
    float currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    if (m_unifTime != -1) {
        glUniform1f(
            m_unifTime, currentTime
            );
    }

    if (m_unifColor != -1) {
        glUniform4fv(
            m_unifColor,
            1,
            &(geo.GetColor()[0])
            );
    }

    Render
    glDrawElements(
        geo.GLDrawMode(),
        geo.ElementCount(),
        GL_UNSIGNED_SHORT, NULL
        );

    geo.DisableVertexAttributes();
}
