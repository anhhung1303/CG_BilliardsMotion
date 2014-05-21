#pragma once
#include "Object.hpp"

#include  "Constant.hpp"
class Ball :
	public Object
{
public:
	Ball();
	~Ball();

	virtual void render(Camera * camera, Light * light, GLdouble elapsedTime);
	// apply new velocity to ball
	void setVelocity(glm::vec3 velocity);
	// get velocity of the ball
	glm::vec3 getVelocity();

	void setRadius(float radius);
	float getRadius();

	void collideWithOtherBall(Ball * otherBall);
	void collideWithTable();
private:
	glm::vec3 velocity;
	glm::vec3 acceleration;

	float radius;

	GLdouble timeLastFrame;
};