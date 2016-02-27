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

void
Scene::InitFromJson()
{

}

void
Scene::Update()
{

}

const Geometry&
Scene::RootGeometry() const
{
	return m_box;
}
