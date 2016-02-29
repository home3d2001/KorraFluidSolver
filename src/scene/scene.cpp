#include <scene/scene.h>
#include <utility.h>

// #define TEST_SCENE

Scene::Scene() :
    Scene(800, 600)
{}

Scene::Scene(
    const float& screenWidth,
    const float& screenHeight
    ) :
    m_testBox(nullptr),
    m_fluidContainer(nullptr),
    m_fluidGeo(nullptr),
    m_fluidSolver(nullptr)
{
    // Initialize camera
    m_camera = new Camera(screenWidth, screenHeight);
}

void
Scene::InitFromTestScene()
{
    m_testBox = new Box();
    m_testBox->SetDrawMode(DrawMode_Wireframe);
    m_testBox->Create();
}

void
Scene::InitFromJson(const char* filepath)
{
    // -- Parse JSON
    Json::Value root;
    bool success = ParseJson(filepath, root);
    if (!success) {
        // Failed parsing JSON
        return;
    }

    // -- Get container information
    const Json::Value containerDimJson = root["containerDim"];

    glm::vec3 containerDim(
        containerDimJson["scaleX"].asFloat(),
        containerDimJson["scaleY"].asFloat(),
        containerDimJson["scaleZ"].asFloat()
        );

    // Create geometry for the container
    m_fluidContainer = new Box();
    m_fluidContainer->Scale(containerDim.x, containerDim.y, containerDim.z);
    m_fluidContainer->SetDrawMode(DrawMode_Wireframe);
    m_fluidContainer->SetColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    m_fluidContainer->Create();

    // -- Get particle information
    const Json::Value particleDimJson = root["particleDim"];
    glm::vec3 particleDim(
        particleDimJson["boundX"].asFloat(),
        particleDimJson["boundY"].asFloat(),
        particleDimJson["boundZ"].asFloat()
        );

    const float separation = root["particleSeparation"].asFloat();

    // -- Initialize fluid solvers
    m_fluidSolver = new FluidSolver(
        containerDim,
        particleDim,
        separation
        );

    // -- Initialize fluid geo
    m_fluidGeo = new FluidGeo(
        m_fluidSolver->ParticlePositions(),
        m_fluidSolver->ParticleVelocities(),
        m_fluidSolver->ParticleSpawnTimes()
        );
    m_fluidGeo->Create();
}

void
Scene::Update(
    const KeyboardControl* kc
    )
{
    UpdateCamera(kc);

#ifdef TEST_SCENE
    // Rotate bounding box
    m_testBox->Rotate(0.02f, 0.01f, 0.02);
    return;
#endif

    UpdateFluidSolver();
}

void
Scene::Draw(
    const ShaderProgram& prog
    ) const
{
#ifdef TEST_SCENE
    prog.Draw(*m_camera, *m_testBox);
    return;
#endif
}

void
Scene::DrawTransformFeedback(
    const ParticleEmitProgram& progUpdate,
    const ParticleDrawProgram& progDraw
    )
{
    DrawFluidSolver(progUpdate, progDraw);
}

void
Scene::CleanUp()
{
    if (m_camera != nullptr) {
        delete m_camera;
    }

    if (m_testBox != nullptr) {
        delete m_testBox;
    }

    if (m_fluidContainer != nullptr) {
        delete m_fluidContainer;
    }

    if (m_fluidGeo != nullptr) {
        delete m_fluidGeo;
    }

    if (m_fluidSolver != nullptr) {
        delete m_fluidSolver;
    }
}

// ----- Private ------ //

void
Scene::UpdateCamera(
    const KeyboardControl* kc
    )
{
    float rotateAmt = 20.0f;
    float zoomAmt = 0.01f;
    if (kc->KeyPressed(Key_Up)) {
        m_camera->RotateAboutUp(rotateAmt);
    }
    if (kc->KeyPressed(Key_Down)) {
        m_camera->RotateAboutUp(-rotateAmt);
    }

    if (kc->KeyPressed(Key_Left)) {
        m_camera->RotateAboutRight(rotateAmt);
    }

    if (kc->KeyPressed(Key_Right)) {
        m_camera->RotateAboutRight(-rotateAmt);
    }

    if (kc->KeyPressed(Key_W)){
        m_camera->Zoom(zoomAmt);
    }

    if (kc->KeyPressed(Key_S)){
        m_camera->Zoom(-zoomAmt);
    }

    if (kc->KeyPressed(Key_D)){
        m_camera->TranslateAlongRight(zoomAmt);
    }

    if (kc->KeyPressed(Key_A)){
        m_camera->TranslateAlongRight(-zoomAmt);
    }
}

void
Scene::UpdateFluidSolver()
{
    m_fluidSolver->Update();
}

void
Scene::DrawFluidSolver(
    const ParticleEmitProgram& progUpdate,
    const ParticleDrawProgram& progDraw
    )
{
    // -- Draw boundary
    progDraw.Draw(*m_camera, *m_fluidContainer);

    // -- Draw particles
    progUpdate.Draw(m_camera, m_fluidGeo);
    m_fluidGeo->ToggleVao();
    progDraw.Draw(*m_camera, *m_fluidGeo);
}
