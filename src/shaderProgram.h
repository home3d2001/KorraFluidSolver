#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "loadShaders.h"
#include "geometry/geometry.h"

class ShaderProgram
{

public:
    ShaderProgram() {}
    ShaderProgram(
        const char* vertFilePath,
        const char* fragFilePath
        );

    void Render(
    	const Geometry* geo
    	);

private:
    GLuint m_programID;
};

#endif
