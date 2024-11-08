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

glm::vec3 Camera::forward() const {
    return glm::normalize(m_target - m_position);
}

void Camera::setTopView()
{
    m_topView = !m_topView;
}

void Camera::setFree() {
    m_followCharacter = false;

}

void Camera::setFollowCharacter(const glm::mat4 modelMatrix)
{
    characterToFollow = modelMatrix;
    m_followCharacter = true;
    glm::vec3 modelPosition = glm::vec3(characterToFollow[3][0], characterToFollow[3][1], characterToFollow[3][2]);
    float distanceBehindWolf = 2.0f;  // Adjust as needed
    glm::vec3 modelForward = glm::vec3(0.0f, 0.0f, 1.0f);
    m_position = modelPosition - modelForward * distanceBehindWolf + glm::vec3(0.0f, 1.0f, 0.0f);
    m_target = modelPosition;

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

void Camera::moveUp() {
    m_position += m_up * cameraSpeed;
    m_target += m_up * cameraSpeed;
}

void Camera::moveDown() {
    m_position -= m_up * cameraSpeed;
    m_target -= m_up * cameraSpeed;
}


void Camera::lookCharacter(const glm::mat4 modelMatrix) {
    glm::vec3 modelPosition = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
    //m_target = modelPosition;
}




void Camera::rotate(const glm::vec3& objectPosition, float degrees, bool up) {

    glm::vec3 objectToRotate;

    if (!m_followCharacter) {
        objectToRotate = m_target;
        
    }
    else {

        objectToRotate = objectPosition;
    }


    glm::vec3 toCamera = m_position - objectToRotate;
    float angleRadians = glm::radians(degrees);
    glm::mat4 rotationMatrix;
    glm::vec3 horizontalAxis = glm::normalize(glm::cross(glm::normalize(toCamera), m_up));
    if (!up) rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, s_yAxis);
    else {
        rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, horizontalAxis);
    }
    glm::vec3 newToCamera = glm::vec3(rotationMatrix * glm::vec4(toCamera, 0.0f));
    m_position = objectToRotate + newToCamera;

    m_target = objectToRotate;
    //m_target = glm::normalize(objectPosition - m_position);
    m_up = glm::vec3(rotationMatrix * glm::vec4(m_up, 0.0f));
}




