#pragma once
#include "Object.hpp"

#include  "Constant.hpp"
class Ball :
	public Object
{
public:
	Ball();
	~Ball();

	virtual void render(glm::mat4 projectionMarix, Camera * camera);
	// apply new velocity to ball
	void setVelocity(glm::vec3 velocity);
	// get velocity of the ball
	glm::vec3 getVelocity();

	void setRadius(float radius);
	float getRadius();
private:
	glm::vec3 velocity;
	glm::vec3 acceleration;

	float radius;

	GLdouble timeLastFrame;
};

