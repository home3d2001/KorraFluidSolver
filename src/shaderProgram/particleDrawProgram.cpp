#include <shaderProgram/particleDrawProgram.h>

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

    if (m_unifModel != -1) {
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
        GL_UNSIGNED_SHORT, NULL
        );

    geo.DisableVertexAttributes();
}
