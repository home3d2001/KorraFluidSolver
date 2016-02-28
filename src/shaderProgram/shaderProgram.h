#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "loadShaders.h"
#include "geometry/geometry.h"
#include "camera/camera.h"

class ShaderProgram
{

public:
    ShaderProgram(
        const char* vertFilePath,
        const char* fragFilePath
        );

    virtual void Draw(
    	const Camera&,
    	const Geometry&
    	) const;

    virtual void CleanUp();

protected:
    GLuint m_programID;

    // Uniform locations
    int m_unifModel;
    int m_unifViewProj;
};

#endif
