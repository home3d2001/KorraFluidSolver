#include <json/json.h>
#include <iostream>
#include <fstream>
#include <string>

#include "scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void
Scene::InitFromTestScene()
{
    m_testBox = Box();
    m_testBox.SetDrawMode(DrawMode_Wireframe);
    m_testBox.Create();
}

bool
Scene::InitFromJson()
{
    // -- Open file
    std::string jsonText;
    std::string line;
    std::ifstream jsonFile("../src/scene/scene.json");
    if (jsonFile.is_open() == false) {
        std::cout << "JSON file not open." << endl;
        return false;
    }
    while (std::getline(jsonFile, line)) {
        jsonText += line;
    }

    // -- Parse json
    Json::Value root;
    Json::Reader reader;
    bool success = reader.parse(jsonText, root, false);
    if (!success)
    {
        std::cout << "Parsing failed." << endl;

        // report to the user the failure and their locations in the document.
        std::cout  << reader.getFormatedErrorMessages() << endl;
        return false;
    }

    // -- Get container information
    const Json::Value containerDim = root["containerDim"];

    m_testBox = Box();
    m_testBox.Scale(
        containerDim["scaleX"].asFloat(),
        containerDim["scaleY"].asFloat(),
        containerDim["scaleZ"].asFloat()
    );
    m_testBox.SetDrawMode(DrawMode_Wireframe);
    m_testBox.Create();

    return true;
}

void
Scene::Update()
{

#ifdef ANIMATE
    // Rotate bounding box
    m_testBox.Rotate(0.02f, 0.01f, 0.02);
#endif

}

const Geometry&
Scene::RootGeometry() const
{
    return m_testBox;
}
