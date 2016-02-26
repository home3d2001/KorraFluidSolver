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
    gWindow = glfwCreateWindow( SCREEN_SIZE.x, SCREEN_SIZE.y, "Trungle Fluid", NULL, NULL);
    if( gWindow == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(gWindow);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(gWindow, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Swap buffers
        glfwSwapBuffers(gWindow);
        glfwPollEvents();

    }
    // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(gWindow, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(gWindow) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

