//
//  viewer.cpp

#include "viewer.h"

Viewer::Viewer()
{
    m_width = 800;
    m_height = 600;

    this->Init();

}

Viewer::Viewer(
    int width,
    int height
    )
{
    m_width = width;
    m_height = height;

    this->Init();
}

void
Viewer::Init()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // antiliasing 4 samples
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Get version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Make sure with don't use old profile

    // Open a window and create its OpenGL context
    m_window = glfwCreateWindow( m_width, m_height, "Trung's Fluid Sim", NULL, NULL);
    if( m_window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        getchar();
        this->CleanUp();
    }

    glfwMakeContextCurrent(m_window);

    // Initialize GLEW

    // *N.B*: Strange we have to do this to make glGenVertexArrays to work?
    //
    // GLEW obtains information on the supported extensions from the graphics driver. Experimental or pre-release drivers, however, might not report every available extension through the standard mechanism, in which case GLEW will report it unsupported. To circumvent this situation, the glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), which ensures that all extensions with valid entry points will be exposed.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        this->CleanUp();
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

    // Create and compile GLSL program from the shaders
    m_program = ShaderProgram("../src/glsl/vert_bare.glsl", "../src/glsl/frag_bare.glsl");

    // Initialize scene
    m_scene = Scene();
    // m_scene.InitFromTestScene();
    m_scene.InitFromJson();

    // Initialize camera
    m_camera = Camera(m_width, m_height);

    // White background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void
Viewer::Update()
{
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_scene.Update();
        m_program.Render(m_camera, m_scene.RootGeometry());

        // Swap buffers
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    // Check if the ESC key was pressed or the window was closed
    while(
        glfwGetKey(m_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(m_window) == 0 );
}

void
Viewer::CleanUp()
{
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

