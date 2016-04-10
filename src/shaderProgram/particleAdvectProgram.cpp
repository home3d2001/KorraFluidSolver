#include <stb_image/stb_image_write.h>
#include <shaderProgram/particleAdvectProgram.h>
#include <geometry/fluidgeo.h>
#include <iostream>

void
FlipPixels(
    GLubyte* pixels,
    int w,
    int h
    )
{
    int i = 0, j = h-1, k;
    while (i < j) {
        GLubyte* ri = &pixels[i];
        GLubyte* rj = &pixels[j];
        for (k = 0; k < w * 3; k++) {
            GLubyte t = ri[k];
            ri[k] = rj[k];
            rj[k] = t;
        }
        i++;
        j--;
    }
}

void
SaveScreenShotBmp(
    int width,
    int height,
    string filePath,
    int frameNumber
    )
{
    GLubyte* pixels = new GLubyte[ 3 * width * height];
    glReadBuffer( GL_BACK );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    FlipPixels(pixels, width, height);
    std::string fileName = filePath + std::to_string(frameNumber) + std::string(".bmp");
    stbi_write_bmp(fileName.c_str(), width, height, 3, pixels);
    delete pixels;
}

ParticleAdvectProgram::ParticleAdvectProgram(
    const char* vertAdvectFilepath,
    const char* fragAdvectFilepath,
    const char* vertDrawFilepath,
    const char* fragDrawFilepath
    )
{
    // -- Load advect program
    GLuint vertShaderID = CreateShader(vertAdvectFilepath, GL_VERTEX_SHADER);
    GLuint fragShaderID = CreateShader(fragAdvectFilepath, GL_FRAGMENT_SHADER);
    GLuint m_programAdvect = glCreateProgram();
    glAttachShader(m_programAdvect, vertShaderID);
    glAttachShader(m_programAdvect, fragShaderID);

    const GLchar* varyings[] = {
        "v_position", "v_velocity", "v_spawntime", "v_color"
    };

    glTransformFeedbackVaryings(
        m_programAdvect,
        sizeof(varyings),
        varyings,
        GL_SEPARATE_ATTRIBS
        );

    glDetachShader(m_programAdvect, vertShaderID);
    glDetachShader(m_programAdvect, fragShaderID);

    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);

    // -- Advect program uniforms
    m_unifAdvectTime = glGetUniformLocation(m_programAdvect, "u_time");
    m_unifAdvectAccel = glGetUniformLocation(m_programAdvect, "u_accel");

    // -- Load draw program
    m_programDraw = LoadShaders(vertDrawFilepath, fragDrawFilepath);

    // -- Draw program uniforms
    m_unifDrawModel = glGetUniformLocation(m_programDraw, "u_model");
    m_unifDrawViewProj = glGetUniformLocation(m_programDraw, "u_viewProj");
    m_unifDrawTime = glGetUniformLocation(m_programDraw, "u_time");
    m_unifDrawMinBoundary = glGetUniformLocation(m_programDraw, "u_minBoundary");
    m_unifDrawMaxBoundary = glGetUniformLocation(m_programDraw, "u_maxBoundary");

    // -- Generate two transform feedback buffers for ping-pong'ing
    glGenTransformFeedbacks(2, &m_TFBuffers[0]);

    // -- Set big point size
    glPointSize(5.0f);
}

void
ParticleAdvectProgram::Advect(
    const float deltaTime,
    FluidGeo* fluidGeo
    )
{
    glUseProgram(m_programAdvect);

    // -- Setup uniforms

    if (m_unifAdvectTime != -1) {
        glUniform1f(
            m_unifAdvectTime, deltaTime
            );
    }

    if (m_unifAdvectAccel != -1) {
        glUniform3f(
            m_unifAdvectAccel, 0.0f, -10.1f, 0.0f
            );
    }

    // -- Bind transform feedback buffer
    if (m_isFirstDraw) {
        glBindTransformFeedback(
            GL_TRANSFORM_FEEDBACK,
            m_TFBuffers[(m_curTFBuffer + 1) & 0x1]
            );

        fluidGeo->ToggleVao();
        fluidGeo->UpdateVao();
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fluidGeo->PosBuffer());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, fluidGeo->VelBuffer());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, fluidGeo->SpawnTimeBuffer());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, fluidGeo->ColBuffer());

        fluidGeo->ToggleVao();

        glBindTransformFeedback(
            GL_TRANSFORM_FEEDBACK,
            m_TFBuffers[m_curTFBuffer]
            );
        fluidGeo->UpdateVao();
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fluidGeo->PosBuffer());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, fluidGeo->VelBuffer());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, fluidGeo->SpawnTimeBuffer());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, fluidGeo->ColBuffer());

    } else {
        glBindTransformFeedback(
            GL_TRANSFORM_FEEDBACK,
            m_TFBuffers[m_curTFBuffer]
            );
    }

    // -- Enable attributes
    fluidGeo->EnableVertexAttributes();

    // Toggle the vao we're using so that we can store the output of
    // transform feedback into the secondary vao
    fluidGeo->ToggleVao();

    // Set transform feedback buffer
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fluidGeo->PosBuffer());
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, fluidGeo->VelBuffer());
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, fluidGeo->SpawnTimeBuffer());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, fluidGeo->ColBuffer());

    // Turn off rasterization - we are not drawing
    glEnable(GL_RASTERIZER_DISCARD);

    // Render
    glBeginTransformFeedback(fluidGeo->GLDrawMode());
    glDrawArrays(fluidGeo->GLDrawMode(), 0, fluidGeo->ElementCount());
    glEndTransformFeedback();

    // Turn on rasterization - we are done
    glDisable(GL_RASTERIZER_DISCARD);
    glUseProgram(NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, NULL);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, NULL);
    fluidGeo->DisableVertexAttributes();

    fluidGeo->ToggleVao();
}

void
ParticleAdvectProgram::Draw(
    const Camera* camera,
    const FluidGeo* fluidGeo,
    const Box* container
    )
{
    glUseProgram(m_programDraw);

    // -- Set uniforms
    if (m_unifDrawModel != -1) {
        glUniformMatrix4fv(
            m_unifDrawModel,
            1,
            GL_FALSE,
            &(fluidGeo->GetLocalTransformation()[0][0])
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

    if (m_unifDrawMinBoundary != -1) {
        glm::vec3 minBoundary = container->GetMinBoundary();
        glUniform3f(
            m_unifDrawMinBoundary,
            minBoundary.x,
            minBoundary.y,
            minBoundary.z
            );
    }

    if (m_unifDrawMaxBoundary != -1) {
        glm::vec3 maxBoundary = container->GetMaxBoundary();
        glUniform3f(
            m_unifDrawMaxBoundary,
            maxBoundary.x,
            maxBoundary.y,
            maxBoundary.z
            );
    }
    // Enable attributes
    fluidGeo->EnableVertexAttributes();

    // Render
    if (m_isFirstDraw)
    {
        glDrawArrays(fluidGeo->GLDrawMode(), 0, fluidGeo->ElementCount());
        // m_isFirstDraw = false;
    }
    else
    {
        glDrawTransformFeedback(
            fluidGeo->GLDrawMode(),
            m_TFBuffers[m_curTFBuffer]);
    }


    // -- Read frame buffer pixels the store to image
    int width = (int)camera->Width() * 2;
    int height = (int)camera->Height() * 2;
    static int frameNumber = 0;
    SaveScreenShotBmp(width, height, "recording/frame_", frameNumber++);

    fluidGeo->DisableVertexAttributes();

    // -- Toggle transform feedback buffer
    m_curTFBuffer = (m_curTFBuffer + 1) & 0x1;
}

void
ParticleAdvectProgram::CleanUp()
{
    glDeleteProgram(m_programAdvect);
    glDeleteProgram(m_programDraw);
}


