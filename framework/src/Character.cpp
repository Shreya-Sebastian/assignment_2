#include "Character.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


Character::Character(const glm::vec3& position): bodyPosition(position), armAngle(0.0f), legAngle(0.0f) {}

void Character::update(float deltaTime) {
    armAngle = sin(glfwGetTime()) * 30.0f;
    legAngle = cos(glfwGetTime()) * 20.0f;
}

void Character::drawCube() const {
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

void Character::draw() const {
    glPushMatrix();
    glTranslatef(bodyPosition.x, bodyPosition.y, bodyPosition.z);

    drawBody();

    //Right arm
    drawArm(armAngle, glm::vec3(0.5f, 0.0f, 0.0f));
    //Left arm
    drawArm(-armAngle, glm::vec3(-0.5f, 0.0f, 0.0f));
    //Right leg
    drawLeg(legAngle, glm::vec3(0.3f, -0.5f, 0.0f));
    //Left legs
    drawLeg(-legAngle, glm::vec3(-0.3f, -0.5f, 0.0f));

    glPopMatrix();
}

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
}
