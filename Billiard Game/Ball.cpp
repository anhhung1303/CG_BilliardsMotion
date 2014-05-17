#include "Ball.hpp"


Ball::Ball()
{
	Object::Object();
	this->timeLastFrame = 0.0;
	radius = 5.7;
}


Ball::~Ball()
{
	Object::~Object();
}

void Ball::render(Camera * camera, Light * light, GLdouble elapsedTime)
{
	if (elapsedTime > Constant::TIME_FOR_A_FRAME){
		float coef = elapsedTime / Constant::TIME_FOR_A_FRAME;
		//cout << "Current time = " << currentTime << " lastTime = " << this->timeLastFrame << " " << Constant::TIME_FOR_A_FRAME << endl;
		//cout << "Coef = " << coef << endl;
		this->translate(velocity.x * coef, velocity.y * coef, velocity.z * coef);

		/*glm::vec3 rotateAxis = glm::vec3(-velocity.z, 0.0f, velocity.x);
		float angle = glm::length(velocity) * coef / this->getRadius();
		this->rotate(angle, rotateAxis);*/

		if (glm::length(this->velocity) != 0){
			//cout << "Current time = " << currentTime << " lastTime = " << this->timeLastFrame << " " << Constant::TIME_FOR_A_FRAME << endl;
			if (glm::length(this->velocity) <= glm::length(this->acceleration)){
				setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
			}
			else {
				setVelocity(this->velocity + this->acceleration);
			}
		}
	}
	Object::render(camera, light, elapsedTime);
}


// apply new velocity to ball
void Ball::setVelocity(glm::vec3 velocity)
{
	//cout << "set ball velocity" << velocity.x << " " << velocity.y << " " << velocity.z << endl;
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

void Ball::collideWithOtherBall(Ball * otherBall){
	float spring = 0.003;

	glm::vec4 otherBallPositionInItsLocal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 otherBallInWorld = otherBall->getModelMatrix() * otherBallPositionInItsLocal;
	glm::vec4 otherBallInBallLocal = glm::inverse(this->getModelMatrix()) * otherBallInWorld;

	float dx = otherBallInBallLocal.x;
	float dz = otherBallInBallLocal.z;
	float distance = sqrt(dx*dx + dz*dz);
	float minDist = 2 * this->getRadius();
	//cout << "distance = " << distance << endl;
	//cout << "minDist = " << minDist << endl;
	if (distance < minDist) {
		float angle = atan2(dz, dx);
		cout << "Angle = " << angle << endl;
		float targetX = cos(angle) * minDist;
		float targetZ = sin(angle) * minDist;
		cout << "targetX = " << targetX << " targetZ = " << targetZ << endl;
		float ax = (targetX - otherBallInBallLocal.x) * spring;
		float az = (targetZ - otherBallInBallLocal.z) * spring;
		cout << "ax = " << ax << " az = " << az << endl;
		this->setVelocity(glm::vec3(this->velocity.x - ax, 0.0f, this->velocity.z - az));
		otherBall->setVelocity(glm::vec3(otherBall->velocity.x + ax, 0.0f, otherBall->velocity.z + az));

		cout << "Collision detected" << endl;
	}
}

void Ball::collideWithTable(){
	float radiusInWorld = this->getRadius() * glm::length(this->getModelMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	//cout << "Radius = " << radiusInWorld << endl;

	glm::vec4 ballPositionInItsLocal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 ballInWorld4 = this->getModelMatrix() * ballPositionInItsLocal;
	glm::vec3 ballInWorld3 = glm::vec3(ballInWorld4.x, ballInWorld4.y, ballInWorld4.z);
	float absZ = (ballInWorld3.z > 0) ? ballInWorld3.z : -ballInWorld3.z;
	//cout << "AbsZ = " << absZ << endl;
	int inverseX = 1, inverseZ = 1;
	glm::vec3 oldPosition = ballInWorld3 - velocity;
	if (absZ + radiusInWorld >= 0.07){
		inverseZ = -1;
		//float coef = -(0.07 - absZ + radiusInWorld) / ((velocity.z >= 0) ? velocity.z : -velocity.z);
		//this->translate(velocity*coef);
		this->translate(velocity*-1.0f);
	}
	float absX = (ballInWorld3.x > 0) ? ballInWorld3.x : -ballInWorld3.x;
	//cout << "absX = " << absX << endl;
	if (absX + radiusInWorld >= 0.143){
		inverseX = -1;
		//float coef = -(0.143 - absX + radiusInWorld) / ((velocity.x >= 0) ? velocity.x : -velocity.x);
		//this->translate(velocity*coef);
		this->translate(velocity*-1.0f);
	}
	this->setVelocity(glm::vec3(inverseX * velocity.x, 0.0f, inverseZ * velocity.z));

}
