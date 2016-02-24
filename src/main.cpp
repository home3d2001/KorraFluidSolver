#include <iostream>

#include "main.hpp"

using namespace std;


// ----------------------
// Constants
// ----------------------

const glm::vec2 SCREEN_SIZE(800, 600);

// ----------------------
// Globals
// ----------------------

GLFWwindow* gWindow = NULL;

int main()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // antiliasing 4 samples
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Get version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Make sure with don't use old profile

    // Open a window and create its OpenGL context
    gWindow = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
    if( gWindow == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(gWindow);

    cout << "Hello world 2" << endl;
    return 0;
}

