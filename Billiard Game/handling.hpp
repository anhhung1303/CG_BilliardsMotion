#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "model3D.hpp"
#include "camera.hpp"
#include "util.hpp"
#include "Ball.hpp"
#include "SceneManager.hpp"

class Keyboard{
private:
	static bool keyBuffer[256];
	static Camera* cam;
	static SceneManager* sceneManager;
	static float coef;

public:
	static void setCamera(Camera* camera);
	static void setSceneManager(SceneManager * newSceneManager);

	static void specialKeyFunc(int key, int x, int y);
	static void keyboardFunc(unsigned char key, int x, int y);
};

class Mouse{
private:
	static Camera* cam;
	static Ball * ball;
	static int windowWidth, windowHeight;
	static glm::ivec2 dragStartPos;

	const static float coef;
	static bool isLeftDown;
	static bool isRightDown;

	static int startRightX, startRightY;
public:
	static void setCamera(Camera* camera);
	static void setWindow(int width, int height);
	static void setControllObject(Ball * newBall);

	static void mouseFunc(int button, int state, int x, int y);
	static void mouseMotionFunc(int x, int y);
	static void mouseWheelFunc(int wheel, int direction, int x, int y);
private:
	static void applyChange(const glm::ivec2& curPos);
	static glm::vec4 getRay(float mouse_x, float mouse_y);
};