#include "camera.h"
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
DISABLE_WARNINGS_POP()

#include <cmath> // For std::cos and std::sin


Camera::Camera(Window* pWindow, const glm::vec3& pos, const glm::vec3& forward)
    : m_position(pos)
    , m_target(forward)
    , m_pWindow(pWindow)
{
}

glm::vec3 Camera::cameraPos() const
{
    return m_position;
}

glm::mat4 Camera::viewMatrix() const
{
    if (!m_topView) return glm::lookAt(m_position, m_target, m_up);
    else return glm::lookAt(glm::vec3(0, 5, -1), glm::vec3(0), m_up);

}

glm::mat4 Camera::rotationMatrix(const glm::mat4& modelMatrix) {
    glm::vec3 modelPosition = glm::vec3(modelMatrix[3]);
    glm::vec3 forward = glm::normalize(m_target - modelPosition);
    glm::vec3 right = glm::normalize(glm::cross(s_yAxis, forward));
    glm::vec3 adjustedUp = glm::cross(forward, right);
    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix[0] = glm::vec4(right, 0.0f);
    rotationMatrix[1] = glm::vec4(adjustedUp, 0.0f);
    rotationMatrix[2] = glm::vec4(forward, 0.0f);
    return rotationMatrix;

}


void Camera::setTopView()
{
    m_topView = !m_topView;
}


void Camera::setFollowCharacter()
{
    m_followCharacter = !m_followCharacter;
}

//moving
void Camera::moveForward() {
    glm::vec3 forward = glm::normalize(m_target - m_position);
    m_position += forward * cameraSpeed;
    m_target += forward * cameraSpeed;
}

void Camera::moveBack() {
    glm::vec3 forward = glm::normalize(m_target - m_position);
    m_position -= forward * cameraSpeed;
    m_target -= forward * cameraSpeed;
}



void Camera::moveLeft() {
    glm::vec3 forward = glm::normalize(m_target - m_position);
    glm::vec3 right = glm::normalize(glm::cross(forward, m_up));
    m_position -= right * cameraSpeed;
    m_target -= right * cameraSpeed;
}

void Camera::moveRight() {
    glm::vec3 forward = glm::normalize(m_target - m_position);
    glm::vec3 right = glm::normalize(glm::cross(forward, m_up));
    m_position += right * cameraSpeed;
    m_target += right * cameraSpeed;
}




void Camera::rotate(const glm::vec3& objectPosition, float degrees, bool up) {

    glm::vec3 toCamera = m_position - objectPosition;
    float angleRadians = glm::radians(degrees);
    glm::mat4 rotationMatrix;
    glm::vec3 horizontalAxis = glm::normalize(glm::cross(glm::normalize(toCamera), m_up));
    if (!up) rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, s_yAxis);
    else {

        rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, horizontalAxis);
    }
    glm::vec3 newToCamera = glm::vec3(rotationMatrix * glm::vec4(toCamera, 0.0f));
    m_position = objectPosition + newToCamera;

    m_target = glm::normalize(objectPosition - m_position);
    m_up = glm::vec3(rotationMatrix * glm::vec4(m_up, 0.0f));
}




