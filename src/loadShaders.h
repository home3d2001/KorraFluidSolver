#ifndef loadShader_h
#define loadShader_h

#include <GL/glew.h>

GLuint CreateShader(const char* filepath, const GLenum shaderType);
GLuint LinkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif /* loadShader_h */
