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
#include "camera/camera.h"

class Viewer
{
public:
    Viewer();
    Viewer(
        int width = 800,
        int height = 600
        );

    virtual void Update();
    virtual void CleanUp();

    float Width() {
        return m_width;
    }

    float Height() {
        return m_height;
    }

protected:
    void Init();

    float m_width;
    float m_height;

    GLFWwindow* m_window;
    ShaderProgram* m_program;
    Scene* m_scene;
};

#endif /* viewer_hpp */
