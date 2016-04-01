#ifndef viewer_hpp
#define viewer_hpp

// #define TEST_SCENE

// C++ libraries

#include <iostream>

// GL libraries

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/button.h>
#include <nanogui/layout.h>
#include <nanogui/textbox.h>
#include <nanogui/label.h>
#include <nanogui/slider.h>
#include <nanogui/combobox.h>
#if defined(_WIN32)
#include <windows.h>
#endif
// #include <nanogui/glutil.h>
#include <iostream>

#include <shaderProgram/shaderProgram.h>
#include <shaderProgram/particleAdvectProgram.h>
#include <input/keyboardControl.h>
#include <scene/scene.h>
#include <camera/camera.h>

using namespace nanogui;

class Viewer :public nanogui::Screen
{
public:
    Viewer(
        int width = 800,
        int height = 600
        );

    virtual void CleanUp();
    virtual void draw(NVGcontext *ctx);
    virtual void drawContents();
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

    float Width() {
        return m_width;
    }

    float Height() {
        return m_height;
    }

protected:
    void Init();
    void InitGUI();
    void ReplayScene();

    float m_width;
    float m_height;
    float m_timeStep = 0.001f;
    float m_fps;
    Label* m_labelFps;

    ShaderProgram* m_program;
    ParticleAdvectProgram* m_programAdvect;
    Scene* m_scene;
};

#endif /* viewer_hpp */
