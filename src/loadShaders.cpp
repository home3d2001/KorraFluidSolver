// @todo: Properly credit OpenGL Tutorials

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include "loadShaders.h"

GLuint
CreateShader(
    const char* filepath,
    const GLenum shaderType
    )
{
    GLuint shaderId = glCreateShader(shaderType);

    // Read shader code from file
    std::string shaderCode;
    std::ifstream shaderStream(filepath, std::ios::in);

    if (shaderStream.is_open()) {
        std::string line = " ";
        while (getline(shaderStream, line)) {
            shaderCode += "\n" + line;
        }
        shaderStream.close();
    } else {
        printf("Can't open");
        getchar();
        return 0;
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    // Compile shader
    printf("Compiling %s shader: %s\n", shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", filepath);
    char const* sourcePointer = shaderCode.c_str();
    glShaderSource(shaderId, 1, &sourcePointer, NULL);
    glCompileShader(shaderId);

    // Check shader
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
        std::vector<char> shaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderId, infoLogLength, NULL, &shaderErrorMessage[0]);
        printf("%s\n", &shaderErrorMessage[0]);
    }

    return shaderId;
}

GLuint
LinkProgram(
    GLuint vertexShaderID,
    GLuint fragmentShaderID
    )
{
    GLint result = GL_FALSE;
    int infoLogLength;

    // Link the program
    GLuint programID = glCreateProgram();
    printf("Linking program\n");
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
        printf("%s\n", &programErrorMessage[0]);
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

GLuint
LoadShaders(
    const char* vertFilePath,
    const char* fragFilePath
    )
{
    GLuint vertexShaderID = CreateShader(vertFilePath, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = CreateShader(fragFilePath, GL_FRAGMENT_SHADER);

    return LinkProgram(vertexShaderID, fragmentShaderID);
}
