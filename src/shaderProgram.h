#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "loadShaders.h"
#include "geometry/geometry.h"
#include "camera/camera.h"

class ShaderProgram
{

public:
    ShaderProgram() {}
    ShaderProgram(
        const char* vertFilePath,
        const char* fragFilePath
        );

    void Render(
    	const Camera&,
    	const Geometry&
    	);

private:
    GLuint m_programID;

    // Uniform locations
    int m_unifModel;
    int m_unifViewProj;
};

#endif
