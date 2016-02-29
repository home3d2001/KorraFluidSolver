#include "viewer.h"

Viewer::Viewer()
    : Viewer(800, 600)
{}

Viewer::Viewer(
    int width,
    int height
    ) : m_window(nullptr),
        m_program(nullptr),
        m_programEmit(nullptr),
        m_scene(nullptr)
{
    m_width = width;
    m_height = height;

    this->Init();
}

void
Viewer::Init()
{
    // -- Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
    }

    // antiliasing 4 samples
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    // Get version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Make sure with don't use old profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // -- Open a window and create its OpenGL context
    m_window = glfwCreateWindow( m_width, m_height, "Trung's Fluid Sim", NULL, NULL);
    if( m_window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        getchar();
        this->CleanUp();
    }

    glfwMakeContextCurrent(m_window);

    // -- Initialize GLEW

    // *N.B*: Strange we have to do this to make glGenVertexArrays to work?
    //
    // GLEW obtains information on the supported extensions from the graphics driver. Experimental or pre-release drivers, however, might not report every available extension through the standard mechanism, in which case GLEW will report it unsupported. To circumvent this situation, the glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), which ensures that all extensions with valid entry points will be exposed.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        this->CleanUp();
    }

    // -- Initialize inputs
    m_keyboard = new KeyboardControl(m_window);

    // -- Initialize shader program
    m_program = new ShaderProgram("../src/glsl/simple.vert", "../src/glsl/simple.frag");
    m_programEmit = new ParticleEmitProgram(
        "../src/glsl/particle_emit.vert",
        "../src/glsl/particle_emit.frag",
        "../src/glsl/particle_draw.vert",
        "../src/glsl/particle_draw.frag"
        );

    // -- Initialize scene
    m_scene = new Scene(m_width, m_height);

    // Have a test scene here in case our graphics pipeline is broken
#ifdef TEST_SCENE
    m_scene->InitFromTestScene();
#else
    m_scene->InitFromJson("../src/scene/scene.json");
#endif

    // -- Clear background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void
Viewer::Update()
{
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_scene->Update(m_keyboard, *m_programEmit);

#ifdef TEST_SCENE
        m_scene->Draw(*m_program);
#else
        m_scene->DrawTransformFeedback(*m_program, *m_programEmit);
#endif

        // Swap buffers
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    while(
        m_keyboard->KeyPressed(Key_Escape) == false &&
        glfwWindowShouldClose(m_window) == 0
        );
}

void
Viewer::CleanUp()
{
    m_program->CleanUp();
    m_programEmit->CleanUp();
    m_scene->CleanUp();

    // *N.B*: Should delete resources in reverse order of creation
    delete m_scene;
    delete m_programEmit;
    delete m_program;
    delete m_keyboard;

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

