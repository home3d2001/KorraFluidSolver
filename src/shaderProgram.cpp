#include "shaderProgram.h"
#include "loadShaders.h"

ShaderProgram::ShaderProgram(
        const char* vertFilePath,
        const char* fragFilePath
        )
{
    m_programID = LoadShaders(vertFilePath, fragFilePath);
}

void ShaderProgram::Render(
    const Geometry* geo
    )
{
    glUseProgram(m_programID);
    geo->EnableVertexAttributes();
    glDrawElements(geo->DrawMode(), geo->ElementCount(), GL_UNSIGNED_SHORT, NULL);
    geo->DisableVertexAttributes();
}
