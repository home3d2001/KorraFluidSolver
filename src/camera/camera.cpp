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
        m_eye(glm::vec3(0.0f, 0.0f, -5.0f)),
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
Camera::EnablePerspective(
    bool enabled
    )
{
    m_isPerspective = enabled;

    if (!m_isPerspective) {
        m_eye = glm::vec3(0.0, 0.0, 10.0);
    } else {
        m_eye = glm::vec3(10.0f, 10.0f, -20.0f);
    }
    RecomputeAttributes();
}

void
Camera::RecomputeAttributes()
{
    m_forward = m_target - m_eye;
    m_right = glm::cross(m_forward, m_up);

    m_view = glm::lookAt(
        m_eye,
        m_target,
        m_up
        );

    if (m_isPerspective) {
        m_proj = glm::perspective(
            m_fov,
            m_width / m_height,
            m_nearClip,
            m_farClip
            );
    } else {
        m_proj = glm::ortho(
            0.0f,
            m_width,
            0.0f,
            m_height
        );
    }
}

glm::mat4
Camera::GetViewProj() const
{
    return m_proj * m_view;
}

void
Camera::RotateAboutRight(
    float deg
    )
{
    deg *= DEG2RAD;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, m_up);
    m_target = m_target - m_eye;
    m_target = glm::vec3(rotation * glm::vec4(m_target, 1.0f));
    m_target = m_target + m_eye;
    RecomputeAttributes();
}

void
Camera::RotateAboutUp(
    float deg
    )
{
    deg *= DEG2RAD;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, m_right);
    m_target = m_target - m_eye;
    m_target = glm::vec3(rotation * glm::vec4(m_target, 1.0f));
    m_target = m_target + m_eye;
    RecomputeAttributes();
}

void
Camera::Zoom(
    float amount
    )
{
    glm::vec3 translation = m_forward * amount;
    m_eye += translation;
    m_target += translation;
    RecomputeAttributes();
}

void
Camera::TranslateAlongRight(
    float amt
    )
{
    glm::vec3 translation = m_right * amt;
    m_eye += translation;
    m_target += translation;
    RecomputeAttributes();
}

void
Camera::TranslateAlongUp(
    float amt
    )
{
    glm::vec3 translation = m_up * amt;
    m_eye += translation;
    m_target += translation;
    RecomputeAttributes();
}
