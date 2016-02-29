#include <shaderProgram/particleEmitProgram.h>
#include <geometry/fluidgeo.h>
#include <iostream>
#include <string>

ParticleEmitProgram::ParticleEmitProgram(
    const char* vertFilePath,
    const char* fragFilePath
) : ShaderProgram(vertFilePath, fragFilePath)
{
    const GLchar* varyings[] = {
        "a_position", "a_velocity", "a_spawntime"
    };

    glTransformFeedbackVaryings(
        m_programID,
        sizeof(varyings),
        varyings,
        GL_SEPARATE_ATTRIBS
        );

    glLinkProgram(m_programID);

    // Get uniform locations
    m_unifTime = glGetUniformLocation(m_programID, "u_time");
    m_unifAccel = glGetUniformLocation(m_programID, "u_accel");

    glGenTransformFeedbacks(1, &m_transformFeedback);
}

void
ParticleEmitProgram::Draw(
    const Camera* camera,
    const FluidGeo* geo
    ) const
{
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback);

    glUseProgram(m_programID);

    // -- Set uniforms
    if (m_unifModel != -1) {
        glUniformMatrix4fv(
            m_unifModel,
            1,
            GL_FALSE,
            &(geo->GetLocalTransformation()[0][0])
            );
    }

    if (m_unifViewProj != -1) {
        glUniformMatrix4fv(
            m_unifViewProj,
            1,
            GL_FALSE,
            &(camera->GetViewProj()[0][0])
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

    if (m_unifAccel != -1) {
        glUniform3f(
            m_unifAccel, 0.0f, -0.001f, 0.0f
            );
    }

    // Enable attributes
    geo->EnableVertexAttributes();

    // Set transform feedback buffer
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, geo->PosBuffer());
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, geo->VelBuffer());
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, geo->SpawnTimeBuffer());

    // Turn off rasterization - we are not drawing
    glEnable(GL_RASTERIZER_DISCARD);

    // Render
    glBeginTransformFeedback(geo->GLDrawMode());
    glDrawTransformFeedback(
        geo->GLDrawMode(),
        m_transformFeedback
        );
    glEndTransformFeedback();

    // Turn on rasterization - we are done
    glDisable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, NULL);

    geo->DisableVertexAttributes();
}
