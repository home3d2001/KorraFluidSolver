#ifndef viewer_hpp
#define viewer_hpp

// C++ libraries

#include <iostream>

// GL libraries

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shaderProgram.h"
#include "scene/scene.h"

class Viewer
{
public:
    glm::vec2 screenSize;

    Viewer();
    Viewer(
        int width = 800,
        int height = 600
        );

    virtual void Render();
    virtual void CleanUp();

private:
    void
    Init();

    GLFWwindow* m_window = NULL;
    ShaderProgram m_program;
    Scene m_scene;

};

#endif /* viewer_hpp */
