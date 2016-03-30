#ifndef KEYBOARD_CONTROL_H
#define KEYBOARD_CONTROL_H

#include <GLFW/glfw3.h>

typedef enum {
    Key_Escape = GLFW_KEY_ESCAPE,
    Key_W = GLFW_KEY_W,
    Key_A = GLFW_KEY_A,
    Key_S = GLFW_KEY_S,
    Key_D = GLFW_KEY_D,
    Key_P = GLFW_KEY_P,
    Key_Up = GLFW_KEY_UP,
    Key_Down = GLFW_KEY_DOWN,
    Key_Left = GLFW_KEY_LEFT,
    Key_Right = GLFW_KEY_RIGHT,
    Key_Space = GLFW_KEY_SPACE
} KeyCode;

class KeyboardControl
{
public:
    KeyboardControl(GLFWwindow*);
    bool KeyPressed(const KeyCode&) const;

private:
    GLFWwindow* m_window;
};

#endif
