#include "camera.h"
#include <mathConstants.h>

Camera::Camera():
    Camera(800, 600)
{
}

Camera::Camera(
    float width,
    float height
    ) : m_width(width),
        m_height(height),
        m_fov(45),
        m_eye(glm::vec3(0.0f, 0.0f, -12.0f)),
        m_target(glm::vec3(0.0f)),
        m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
        m_right(glm::vec3(1.0f, 0.0f, 0.0f)),
        m_nearClip(1.0f),
        m_farClip(1000.0f)
{
    m_forward = m_target - m_eye;
    RecomputeAttributes();
}

void
Camera::RecomputeAttributes()
{
    m_view = glm::lookAt(
        m_eye,
        m_target,
        m_up
        );

    m_perspective = glm::perspective(
        m_fov,
        m_width / m_height,
        m_nearClip,
        m_farClip
        );
}

glm::mat4
Camera::GetViewProj() const
{
    return m_perspective * m_view;
}

void
Camera::RotateAboutRight(
    float deg
    )
{

}



