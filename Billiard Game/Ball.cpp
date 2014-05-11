#include "Ball.hpp"


Ball::Ball()
{
	Object::Object();
	this->timeLastFrame = 0.0;
}



Ball::~Ball()
{
	Object::~Object();
}

void Ball::render(glm::mat4 projectionMarix, Camera * camera)
{
	GLdouble currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (currentTime - this->timeLastFrame > Constant::TIME_FOR_A_FRAME){
		float coef = (currentTime - this->timeLastFrame) / Constant::TIME_FOR_A_FRAME;
		//cout << "Current time = " << currentTime << " lastTime = " << this->timeLastFrame << " " << Constant::TIME_FOR_A_FRAME << endl;
		//cout << "Coef = " << coef << endl;
		this->translate(velocity.x * coef, velocity.y * coef, velocity.z * coef);

		/*glm::vec3 rotateAxis = glm::vec3(-velocity.z, 0.0f, velocity.x);
		float angle = glm::length(velocity) * coef / this->getRadius();
		this->rotate(angle, rotateAxis);*/

		if (glm::length(this->velocity) != 0){
			cout << "Current time = " << currentTime << " lastTime = " << this->timeLastFrame << " " << Constant::TIME_FOR_A_FRAME << endl;
			if (glm::length(this->velocity) <= glm::length(this->acceleration)){
				setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
			}
			else {
				setVelocity(this->velocity + this->acceleration);
			}
		}
	}
	Object::render(projectionMarix, camera);
}


// apply new velocity to ball
void Ball::setVelocity(glm::vec3 velocity)
{
	cout << "set ball velocity" << velocity.x << " " << velocity.y << " " << velocity.z << endl;
	this->velocity = velocity;
	if (glm::length(this->velocity) > 0){
		this->acceleration = glm::normalize(this->velocity) * Constant::FRICTIONAL_COEFFICIENT;
		this->timeLastFrame = glutGet(GLUT_ELAPSED_TIME);
		//cout << "Last time = " << this->timeLastFrame << endl;
	}
	else {
		this->acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		this->timeLastFrame = 0.0;
	}
}


// get velocity of the ball
glm::vec3 Ball::getVelocity()
{
	return this->velocity;
}

void Ball::setRadius(float radius){
	this->radius = radius;
}
float Ball::getRadius(){
	return this->radius;
}
