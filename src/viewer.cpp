#include "viewer.h"

Viewer::Viewer(
    int width,
    int height
    ) : nanogui::Screen(Eigen::Vector2i(width, height), "Korra Fluid Simulation"),
        m_program(nullptr),
        m_programAdvect(nullptr),
        m_scene(nullptr),
        m_labelFps(nullptr),
        m_checkBoxSolid1(nullptr),
        m_checkBoxSolid2(nullptr)
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

    // -- Initialize shader program
    m_program = new ShaderProgram("../src/glsl/simple.vert", "../src/glsl/simple.frag");
    m_programAdvect = new ParticleAdvectProgram(
        "../src/glsl/particle_advect.vert",
        "../src/glsl/particle_advect.frag",
        "../src/glsl/particle_draw.vert",
        "../src/glsl/particle_draw.frag"
        );

    // -- Initialize scene
    m_scene = new Scene(m_width, m_height);

    // Have a test scene here in case our graphics pipeline is broken
// #define TEST_SCENE
#ifdef TEST_SCENE
    m_scene->InitFromTestScene();
#else
    m_scene->InitFromJson("../src/scene/scene.json");
#endif

    // -- Initialize GUI
    InitGUI();
}

void
Viewer::InitGUI()
{
    Window* toolbox = new Window(this, "Bending parameters");
    toolbox->setPosition(Vector2i(20, 20));
    toolbox->setLayout(new GroupLayout());

    // -- Diagnostics
    Widget* widget = new Widget(toolbox);
    GridLayout *layout =
        new GridLayout(
            Orientation::Horizontal,
            2,
            Alignment::Middle,
            15,
            5);
    layout->setColAlignment(
        { Alignment::Maximum, Alignment::Fill });
    layout->setSpacing(0, 10);
    widget->setLayout(layout);

    // FPS
    // new Label(widget, "FPS: ", "sans-bold");
    // m_labelFps = new Label(widget, to_string(m_fps), "sans-bold");

    new Label(widget, "Number of particles: ", "sans-bold");
    new Label(widget, to_string(m_scene->NumParticles()), "sans-bold");

    // Paricle infos

    // -- Parameter widgets
    widget = new Widget(toolbox);
    layout =
        new GridLayout(
            Orientation::Horizontal,
            3,
            Alignment::Middle,
            15,
            5);
    layout->setColAlignment(
        { Alignment::Maximum, Alignment::Fill });
    layout->setSpacing(0, 10);
    widget->setLayout(layout);

    // Global constants

    new Label(widget, "Mass: ", "sans-bold");
    Slider *slider = new Slider(widget);
    Label* label = new Label(widget, "0.125", "sans-bold");
    slider->setValue(0.0f);
    slider->setCallback([this, label](float value) {
        float mass = value + 0.125f;
        m_scene->SetConstant(SPHMass, mass);
        label->setCaption(to_string(mass));
    });

    new Label(widget, "Stiffness: ", "sans-bold");
    slider = new Slider(widget);
    label = new Label(widget, "10", "sans-bold");
    slider->setValue(0.f);
    slider->setCallback([this, label](float value) {
        float stiffness = value * 1000.0f + 10.f;
        m_scene->SetConstant(SPHStiffness, stiffness);
        label->setCaption(to_string(stiffness));
    });

    new Label(widget, "Viscosity: ", "sans-bold");
    slider = new Slider(widget);
    label = new Label(widget, "0", "sans-bold");
    slider->setValue(0.0f);
    slider->setCallback([this, label](float value) {
        float viscosity = value * 10.0f;
        m_scene->SetConstant(SPHViscosity, viscosity);
        label->setCaption(to_string(viscosity));
    });

    new Label(widget, "Rest Density: ", "sans-bold");
    slider = new Slider(widget);
    label = new Label(widget, "1000", "sans-bold");
    slider->setValue(0.5f);
    slider->setCallback([this, label](float value) {
        float restDensity = value * 2000.0f;
        m_scene->SetConstant(SPHRestDensity, restDensity);
        label->setCaption(to_string(restDensity));
    });

    new Label(widget, "Time step: ", "sans-bold");
    slider = new Slider(widget);
    label = new Label(widget, "0.001", "sans-bold");
    slider->setValue(0.1f);
    slider->setCallback([this, label](float value) {
        m_timeStep = value / 100.0f + 0.001f;
        label->setCaption(to_string(m_timeStep));
    });

    m_checkBoxSolid1 = new CheckBox(widget, "Test box 1");
    m_checkBoxSolid1->setCallback([this](bool state) {
        m_scene->EnableTestBoxOne(state);
    });

    m_checkBoxSolid2 = new CheckBox(widget, "Test box 2");
    m_checkBoxSolid2->setCallback([this](bool state) {
        m_scene->EnableTestBoxTwo(state);
    });

    // -- Coloring particles
    widget = new Widget(toolbox);
    widget->setLayout(new GroupLayout());
    new Label(widget, "Particle color", "sans-bold");
    ComboBox* cb = new ComboBox(widget, { "Blue", "Pressure", "Viscosity", "Velocity", "All forces"});
    cb->setCallback([this](int index) {
        FluidParticle::colorType = (SPHColor)index;
    });

    // -- Buttons
    widget = new Widget(toolbox);
    widget->setLayout(new GroupLayout());

    // Pause
    Button* b = new Button(widget, "Pause");
    b->setCallback([this] {
        m_scene->Pause();
    });

    // Replay
    b = new Button(widget, "Replay");
    b->setCallback([this] {
        this->ReplayScene();
    });

    // Quit
    b = new Button(widget, "Quit");
    b->setCallback([this] {
        this->CleanUp();
    });

    performLayout(mNVGContext);
}

void
Viewer::ReplayScene()
{
    if (m_scene != nullptr) {
        delete m_scene;
    }
    m_scene = new Scene(m_width, m_height);
    m_scene->InitFromJson("../src/scene/scene.json");

    // -- Update scene based on current state of GUI
    m_scene->EnableTestBoxOne(m_checkBoxSolid1->checked());
    m_scene->EnableTestBoxTwo(m_checkBoxSolid2->checked());
}

void
Viewer::draw(
    NVGcontext *ctx
    )
{
    Screen::draw(ctx);
}

void
Viewer::drawContents()
{
    // -- Update fps
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    double deltaTime = double(currentTime - lastTime);
    m_fps = 1000.0 / deltaTime;
    // m_labelFps->setCaption(to_string(m_fps));

#ifdef TEST_SCENE
    m_scene->Draw(*m_program);
#else
    m_scene->Update(m_timeStep, *m_programAdvect);
    m_scene->DrawTransformFeedback(*m_program, *m_programAdvect);
#endif

}

bool
Viewer::keyboardEvent(
    int key,
    int scancode,
    int action,
    int modifiers
    )
{
    if (Screen::keyboardEvent(key, scancode, action, modifiers))
        return true;

    // Quit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        this->CleanUp();
        return true;
    }

    // Replay
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        this->ReplayScene();
    }

    m_scene->ReadInputs((KeyCode)key, (KeyAction)action);
    return false;
}

void
Viewer::CleanUp()
{
    m_program->CleanUp();
    m_programAdvect->CleanUp();
    m_scene->CleanUp();

    // *N.B*: Should delete resources in reverse order of creation
    delete m_scene;
    delete m_programAdvect;
    delete m_program;

    setVisible(false);
    nanogui::shutdown();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

