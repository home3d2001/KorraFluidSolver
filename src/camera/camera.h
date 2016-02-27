#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    Camera(float width, float height);

    // -- Camera attributes
    glm::mat4 GetViewProj() const;
    void RecomputeAttributes();

    // -- Camera movement
    void RotateAboutRight(float deg);
    void RotateAboutUp(float deg);
    void Zoom(float amount);

protected:

    // -- Attributes
    float m_width;
    float m_height;
    float m_fov;
    glm::vec3 m_eye;
    glm::vec3 m_target;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::vec3 m_right;
    float m_nearClip;
    float m_farClip;

    glm::mat4 m_view;
    glm::mat4 m_perspective;
};


#endif /* CAMERA_H */
