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
	m_box.Create();
}

void
Scene::InitFromJson()
{

}

const Geometry&
Scene::RootGeometry() const
{
	return m_box;
}
