#include "Character.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
//#include <glad/glad.h>


Character::Character(const glm::vec3& position): bodyPosition(position), armAngle(0.0f), legAngle(0.0f) {}

void Character::update(float deltaTime) {
    armAngle = sin(glfwGetTime()) * 30.0f;
    legAngle = cos(glfwGetTime()) * 20.0f;
}

void Character::drawCube(const Shader& shader) const
{
    glBegin(GL_QUADS);

    //Front face, towards pos Z-axis
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    
    //Back face towards neg Z-axis
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    //Left face towards neg X-axis
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    //Right face
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    //Top face
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    //Bottom face
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    glEnd();
}

void Character::draw(const Shader& shader, const glm::mat4& parentTransform) const
{
    shader.bind();

    // Body
    glm::mat4 bodyTransform = parentTransform * glm::translate(glm::mat4(1.0f), bodyPosition);
    glm::mat3 normalModelMatrix = glm::transpose(glm::inverse(glm::mat3(bodyTransform)));
    glUniformMatrix4fv(shader.getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(bodyTransform));
    glUniformMatrix3fv(shader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
    drawCube(shader);

    // Right arm
    glm::mat4 rightArmTransform = bodyTransform * glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
    rightArmTransform = glm::rotate(rightArmTransform, glm::radians(armAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    normalModelMatrix = glm::transpose(glm::inverse(glm::mat3(rightArmTransform)));
    glUniformMatrix4fv(shader.getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(rightArmTransform));
    glUniformMatrix3fv(shader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
    drawCube(shader);

    // Left arm
    glm::mat4 leftArmTransform = bodyTransform * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
    leftArmTransform = glm::rotate(leftArmTransform, glm::radians(-armAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    normalModelMatrix = glm::transpose(glm::inverse(glm::mat3(leftArmTransform)));
    glUniformMatrix4fv(shader.getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(leftArmTransform));
    glUniformMatrix3fv(shader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
    drawCube(shader);

    // Right leg
    glm::mat4 rightLegTransform = bodyTransform * glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, -0.5f, 0.0f));
    rightLegTransform = glm::rotate(rightLegTransform, glm::radians(legAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    normalModelMatrix = glm::transpose(glm::inverse(glm::mat3(rightLegTransform)));
    glUniformMatrix4fv(shader.getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(rightLegTransform));
    glUniformMatrix3fv(shader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
    drawCube(shader);

    // Left leg
    glm::mat4 leftLegTransform = bodyTransform * glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -0.5f, 0.0f));
    leftLegTransform = glm::rotate(leftLegTransform, glm::radians(-legAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    normalModelMatrix = glm::transpose(glm::inverse(glm::mat3(leftLegTransform)));
    glUniformMatrix4fv(shader.getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(leftLegTransform));
    glUniformMatrix3fv(shader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
    drawCube(shader);
}

/*
void Character::drawBody() const {
    glPushMatrix();
    glScalef(0.5f, 1.0f, 0.5f);
    drawCube();
    glPopMatrix();
}

void Character::drawArm(float angle, const glm::vec3& offset) const {
    glPushMatrix();
    glTranslatef(offset.x, offset.y, offset.z);
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    drawCube();
    glTranslatef(0.5f, 0.0f, 0.0f);
    glRotatef(angle / 2.0f, 1.0f, 0.0f, 0.0f);
    drawCube();
    glPopMatrix();
}

void Character::drawLeg(float angle, const glm::vec3& offset) const {
    glPushMatrix();
    glTranslatef(offset.x, offset.y, offset.z);
    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    drawCube();
    glTranslatef(0.6f, 0.0f, 0.0f);
    glRotatef(angle / 2.0f, 1.0f, 0.0f, 0.0f);
    drawCube();
    glPopMatrix();
}*/
