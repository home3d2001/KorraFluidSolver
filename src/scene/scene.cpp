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
    m_box = Box();

    glm::mat4 rot = glm::rotate(
        glm::mat4(1.0f),
        0.02f,
        glm::vec3(1.0f, 1.0f, 0.0f)
        );

    glm::mat4 scale = glm::scale(
        glm::mat4(1.0f),
        glm::vec3(2.5f, 2.5f, 2.5f)
        );

    glm::mat4 translate = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f)
        );

    m_box.SetLocalTransformation(translate * rot * scale);
    m_box.SetDrawMode(DrawMode_Wireframe);
    m_box.Create();
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

    m_box = Box();
    m_box.Scale(
        containerDim["scaleX"].asFloat(),
        containerDim["scaleY"].asFloat(),
        containerDim["scaleZ"].asFloat()
    );
    m_box.SetDrawMode(DrawMode_Wireframe);
    m_box.Create();

    return true;
}

void
Scene::Update()
{

#ifdef ANIMATE
    // Rotate bounding box
    m_box.Rotate(0.02f, 0.01f, 0.02);
#endif

}

const Geometry&
Scene::RootGeometry() const
{
    return m_box;
}
