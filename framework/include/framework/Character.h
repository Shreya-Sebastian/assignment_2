#ifndef CHARACTER_H
#define CHARACTER_H

#include <glm/glm.hpp>

class Character {
public:
	Character(const glm::vec3& position);

	void update(float deltaTime);
	void draw() const;

private:
	glm::vec3 bodyPosition;
	float armAngle;
	float legAngle;

	void drawBody() const;
	void drawArm(float angle, const glm::vec3& offset) const;
	void drawLeg(float angle, const glm::vec3& offset) const;
	void drawExtremitySegment(float length) const;
	void drawCube() const;

};

#endif