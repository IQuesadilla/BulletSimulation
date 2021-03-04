#pragma once

#define GL_GLEXT_PROTOTYPES
#define GLM_ENABLE_EXPERIMENTAL

#include "gameinit.h"
#include "gameloop.h"

#include "sdlwrapper/window.h"
#include "sdlwrapper/glcontext.h"
#include "sdlwrapper/event.h"
#include "camera/camera.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct gamedata
{
	gamedata(int argc, char **argv);
	~gamedata();

	window _window;
	glcontext _glcontext;
	event _event;
	Camera _camera;

	btDefaultCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btBroadphaseInterface *overlappingPairCache;
	btSequentialImpulseConstraintSolver *solver;
	btDiscreteDynamicsWorld *dynamicsWorld;

	bool firstMouse = true;
	int lastX, lastY;
	bool quit = false;
};