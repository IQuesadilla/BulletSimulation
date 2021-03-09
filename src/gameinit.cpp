#include "gameinit.h"

int gameinit(gamedata &_gamedata, int argc, char **argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	window _window ("win", WINDOWPOS_UNDEFINED, {WINDOW_WIDTH, WINDOW_HEIGHT}, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	glcontext _glcontext (_window);
	glViewport (0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	_gamedata._camera = *new Camera(glm::vec3(0.0f,30.0f,5.0f));
	_gamedata._camera.setViewSize(WINDOW_WIDTH,WINDOW_HEIGHT);

	_gamedata._event.addCallback(SDL_KEYDOWN,keyboard_callback,&_gamedata);
	_gamedata._event.addCallback(SDL_KEYUP,keyboard_callback,&_gamedata);
	_gamedata._event.addCallback(SDL_MOUSEMOTION,mouse_callback,&_gamedata);

	_gamedata.collisionConfiguration = new btDefaultCollisionConfiguration();
	_gamedata.dispatcher = new btCollisionDispatcher(_gamedata.collisionConfiguration);
	_gamedata.overlappingPairCache = new btAxisSweep3({0.0f,0.0f,0.0f},{500.0f,500.0f,500.0f});
	_gamedata.solver = new btSequentialImpulseConstraintSolver;
	
	_gamedata.dynamicsWorld = new btDiscreteDynamicsWorld(_gamedata.dispatcher, _gamedata.overlappingPairCache, _gamedata.solver, _gamedata.collisionConfiguration);

	_gamedata.dynamicsWorld->setGravity(btVector3(0, -10, 0));

	createObjectFromXML(_gamedata.dynamicsWorld,"assets/objects/xmliron.xml");

	return 1;
}

int gamekill()
{
	return 1;
}