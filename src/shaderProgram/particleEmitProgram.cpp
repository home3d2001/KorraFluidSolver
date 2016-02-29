#include <shaderProgram/particleEmitProgram.h>
#include <geometry/fluidgeo.h>
#include <iostream>
#include <string>

ParticleEmitProgram::ParticleEmitProgram(
    const char* vertEmitFilepath,
    const char* fragEmitFilepath,
    const char* vertDrawFilepath,
    const char* fragDrawFilepath
    )
{
    // -- Load emit program
    m_programEmit = LoadShaders(vertEmitFilepath, fragEmitFilepath);

    const GLchar* varyings[] = {
        "a_position", "a_velocity", "a_spawntime"
    };

    glTransformFeedbackVaryings(
        m_programEmit,
        sizeof(varyings),
        varyings,
        GL_SEPARATE_ATTRIBS
        );

    glLinkProgram(m_programEmit);

    // -- Emit program uniforms
    m_unifEmitTime = glGetUniformLocation(m_programEmit, "u_time");
    m_unifEmitAccel = glGetUniformLocation(m_programEmit, "u_accel");

    // -- Load draw program
    m_programDraw = LoadShaders(vertDrawFilepath, fragDrawFilepath);

    // -- Draw program uniforms
    m_unifDrawModel = glGetUniformLocation(m_programDraw, "u_model");
    m_unifDrawViewProj = glGetUniformLocation(m_programDraw, "u_viewProj");
    m_unifDrawTime = glGetUniformLocation(m_programDraw, "u_time");
    m_unifDrawColor = glGetUniformLocation(m_programDraw, "u_color");

    // -- Generate two transform feedback buffers for ping-pong'ing
    glGenTransformFeedbacks(2, &m_TFBuffers[0]);
}

void
ParticleEmitProgram::Emit(
    FluidGeo* geo
    )
{
    glUseProgram(m_programEmit);

    // -- Setup uniforms
    // @todo: maybe we only need to pass delta time through
    static float lastTime = glfwGetTime();
    float currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    if (m_unifEmitTime != -1) {
        glUniform1f(
            m_unifEmitTime, currentTime
            );
    }

    if (m_unifEmitAccel != -1) {
        glUniform3f(
            m_unifEmitAccel, 0.0f, -10.1f, 0.0f
            );
    }

    // -- Bind transform feedback buffer
    glBindTransformFeedback(
        GL_TRANSFORM_FEEDBACK,
        m_TFBuffers[m_curTFBuffer]
        );

    // -- Enable attributes
    geo->EnableVertexAttributes();

    // Toggle the vao we're using so that we can store the output of
    // transform feedback into the secondary vao
    geo->ToggleVao();

    // Set transform feedback buffer
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, geo->PosBuffer());
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, geo->VelBuffer());
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, geo->SpawnTimeBuffer());

    // Turn off rasterization - we are not drawing
    glEnable(GL_RASTERIZER_DISCARD);

    // Render
    glBeginTransformFeedback(geo->GLDrawMode());
    glDrawArrays(geo->GLDrawMode(), 0, geo->ElementCount());
    glEndTransformFeedback();

    // Turn on rasterization - we are done
    glDisable(GL_RASTERIZER_DISCARD);
    glUseProgram(NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, NULL);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, NULL);
    geo->DisableVertexAttributes();
}

void
ParticleEmitProgram::Draw(
    const Camera* camera,
    const FluidGeo* geo
    )
{
    glUseProgram(m_programDraw);

    // Enable attributes
    geo->EnableVertexAttributes();

    // -- Set uniforms
    if (m_unifDrawModel != -1) {
        glUniformMatrix4fv(
            m_unifDrawModel,
            1,
            GL_FALSE,
            &(geo->GetLocalTransformation()[0][0])
            );
    }

    if (m_unifDrawViewProj != -1) {
        glUniformMatrix4fv(
            m_unifDrawViewProj,
            1,
            GL_FALSE,
            &(camera->GetViewProj()[0][0])
            );
    }

    // @todo: maybe we only need to pass delta time through
    static float lastTime = glfwGetTime();
    float currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    if (m_unifDrawTime != -1) {
        glUniform1f(
            m_unifDrawTime, currentTime
            );
    }

    if (m_unifDrawColor != -1) {
        glUniform4fv(
            m_unifDrawColor,
            1,
            &(geo->GetColor()[0])
            );
    }

    // Render
    glDrawElements(
        geo->GLDrawMode(),
        geo->ElementCount(),
        GL_UNSIGNED_SHORT, NULL
        );
    // glDrawTransformFeedback(geo->GLDrawMode(), m_TFBuffers[m_curTFBuffer]);
    geo->DisableVertexAttributes();

    // -- Toggle transform feedback buffer
    m_curTFBuffer = (m_curTFBuffer + 1) % 2;
}

void
ParticleEmitProgram::CleanUp()
{
    glDeleteProgram(m_programEmit);
    glDeleteProgram(m_programDraw);
}


