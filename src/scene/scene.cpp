#include <scene/scene.h>
#include <utility.h>

Scene::Scene() :
    Scene(800, 600)
{}

Scene::Scene(
    const float& screenWidth,
    const float& screenHeight
    ) :
    m_testBox(nullptr),
    m_testBoxV(nullptr),
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
    m_testBox = new Box(DrawMode_Wireframe);
    m_testBox->SetDrawMode(DrawMode_Wireframe);
    m_testBox->Create();
}

void
Scene::InitFromJson(
    const char* filepath
    )
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
    m_fluidContainer = new Box(DrawMode_Wireframe);
    m_fluidContainer->Scale(containerDim.x, containerDim.y, containerDim.z);
    m_fluidContainer->Create();

    // -- Get particle information
    const Json::Value particleDimJson = root["particleDim"];
    glm::vec3 particleDim(
        particleDimJson["boundX"].asFloat(),
        particleDimJson["boundY"].asFloat(),
        particleDimJson["boundZ"].asFloat()
        );

    const float separation = root["particleSeparation"].asFloat();
    const float cellSize = root["cellSize"].asFloat();
    const float stiffness = root["stiffness"].asFloat();
    const float viscosity = root["viscosity"].asFloat();
    const float mass = root["mass"].asFloat();
    const float restDensity = root["restDensity"].asFloat();

    // -- Initialize fluid solvers
    m_fluidSolver = new SPHSolver(
        containerDim,
        particleDim,
        separation,
        cellSize,
        stiffness,
        viscosity,
        mass,
        restDensity
        );

    // -- Initialize fluid geo
    m_fluidGeo = new FluidGeo(
        m_fluidSolver->ParticlePositions(),
        m_fluidSolver->ParticleVelocities(),
        m_fluidSolver->ParticleSpawnTimes(),
        m_fluidSolver->ParticleColors()
        );
    m_fluidGeo->Create();

    // -- Solid object
    if (m_enableTestBox1) {
        m_testBox = new Box(DrawMode_Shaded);
        m_testBox->Translate(0.0f, -1.0f, 0.1f);
        m_testBox->Scale(1.0f, 0.2f, 0.9f);
        m_testBox->SetColor(glm::vec4(0.8f, 0.3f, 0.2f, 1.0f));
        m_testBox->Create();
    }

    if (m_enableTestBox2) {
        m_testBoxV = new Box(DrawMode_Shaded);
        m_testBoxV->Translate(-1.0f, 0.0f, 0.0f);
        m_testBoxV->Scale(0.2f, 1.0f, 0.9f);
        m_testBoxV->SetColor(glm::vec4(0.3f, 0.8f, 0.2f, 1.0f));
        m_testBoxV->Create();
    }
}

void
Scene::SetConstant(
    SPHConstantType type,
    float value
    )
{
    if (m_fluidSolver) {
        m_fluidSolver->SetConstant(type, value);
    }
}

void
Scene::ReadInputs(
    KeyCode key,
    KeyAction action
    )
{
    if (key == Key_Space && action == Key_Pressed) {
        this->Pause();
    }

    if (action == Key_Repeat) {
        UpdateCamera(key);
    }
}

void
Scene::Pause()
{
    m_paused = !m_paused;
}

void
Scene::Update(
    const float deltaT,
    ParticleAdvectProgram& progAdvect
    )
{
    if (m_paused) {
        return;
    }
    // Rotate solid
    if (m_enableTestBox1) {
        m_testBox->Rotate(0.0f, deltaT * 100.0f, 0.0f);
        m_testBox->Translate(0.0f, cos(deltaT) * 0.05f, 0.0f);
    }

    if (m_enableTestBox2) {
        m_testBoxV->Translate(cos(deltaT) * 0.05f, 0.0f, 0.0f);
    }

    // Update solver
    UpdateFluidSolver(deltaT, progAdvect);
}

void
Scene::Draw(
    const ShaderProgram& prog
    ) const
{
    prog.Draw(*m_camera, *m_testBox);
}

void
Scene::DrawTransformFeedback(
    const ShaderProgram& prog,
    ParticleAdvectProgram& progAdvect
    )
{
    DrawFluidSolver(prog, progAdvect);
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
    KeyCode key
    )
{
    // Camera mode
    if (key == Key_P) {
        static bool perspective = true;
        perspective = !perspective;
        m_camera->EnablePerspective(perspective);
    }

    // Camera movement
    float rotateAmt = 20.0f;
    float zoomAmt = 0.01f;
    if (key == Key_Up) {
        m_camera->RotateAboutUp(rotateAmt);
    }
    if (key == Key_Down) {
        m_camera->RotateAboutUp(-rotateAmt);
    }

    if (key == Key_Left) {
        m_camera->RotateAboutRight(rotateAmt);
    }

    if (key == Key_Right) {
        m_camera->RotateAboutRight(-rotateAmt);
    }

    if (key == Key_W){
        m_camera->Zoom(zoomAmt);
    }

    if (key == Key_S){
        m_camera->Zoom(-zoomAmt);
    }

    if (key == Key_D){
        m_camera->TranslateAlongRight(zoomAmt);
    }

    if (key == Key_A){
        m_camera->TranslateAlongRight(-zoomAmt);
    }
}

void
Scene::UpdateFluidSolver(
    const float deltaT,
    ParticleAdvectProgram& progAdvect
    )
{
    // -- Test box 1
    if (m_enableTestBox1) {
        m_fluidSolver->CheckBoxIntersection(m_testBox);
    }

    // -- Test box 2
    if (m_enableTestBox2) {
        m_fluidSolver->CheckBoxIntersection(m_testBoxV);
    }

    m_fluidSolver->Update(deltaT);
    m_fluidGeo->SetVelocities(m_fluidSolver->ParticleVelocities());
    m_fluidGeo->SetPositions(m_fluidSolver->ParticlePositions());

    // Advect particle?
    progAdvect.Advect(deltaT, m_fluidGeo);
}

void
Scene::DrawFluidSolver(
    const ShaderProgram& prog,
    ParticleAdvectProgram& progAdvect
    )
{
    // -- Draw container
    prog.Draw(*m_camera, *m_fluidContainer);

    // -- Draw solids
    if (m_enableTestBox1) {
        prog.Draw(*m_camera, *m_testBox);
    }

    if (m_enableTestBox2) {
        prog.Draw(*m_camera, *m_testBoxV);
    }

    // -- Draw particles
    m_fluidGeo->SetColors(m_fluidSolver->ParticleColors());
    progAdvect.Draw(m_camera, m_fluidGeo, m_fluidContainer);

}
