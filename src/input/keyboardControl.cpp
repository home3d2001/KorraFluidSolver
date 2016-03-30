#include "keyboardControl.h"

KeyboardControl::KeyboardControl(
    GLFWwindow* window
    ) : m_window(window)
{
    // Ensure we can capture key presses
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
}

bool
KeyboardControl::KeyPressed(
    const KeyCode& keyCode
    ) const
{
    return glfwGetKey(m_window, keyCode) == GLFW_PRESS;
}
