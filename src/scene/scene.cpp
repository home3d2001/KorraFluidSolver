#include <scene/scene.h>
#include <utility.h>

Scene::Scene() :
    m_testBox(nullptr),
    m_fluidContainer(nullptr),
    m_fluidGeo(nullptr),
    m_fluidSolver(nullptr)
{
}

Scene::~Scene()
{
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
    const Json::Value containerDim = root["containerDim"];

    m_testBox = new Box();
    m_testBox->Scale(
        containerDim["scaleX"].asFloat(),
        containerDim["scaleY"].asFloat(),
        containerDim["scaleZ"].asFloat()
    );
    m_testBox->SetDrawMode(DrawMode_Wireframe);
    m_testBox->Create();
}

void
Scene::Update()
{

#ifdef ANIMATE
    // Rotate bounding box
    m_testBox->Rotate(0.02f, 0.01f, 0.02);
#endif

}

const Geometry*
Scene::RootGeometry() const
{
    return m_testBox;
}
