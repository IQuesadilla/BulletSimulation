#include "gamemain.h"

int gamedata::gameinit(int argc, char **argv)
{
	app = new QApplication(argc, argv);
	window = new WindowWidget();
	glcontext = new GLWidget();
	_eventhandler = new eventhandler(this);
	window->setGLContextM(glcontext);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glEnable(GL_PROGRAM_POINT_SIZE);

	_camera = *new Camera(glm::vec3(0.0f,30.0f,5.0f));
	_camera.setViewSize(WINDOW_WIDTH,WINDOW_HEIGHT);

	window->_eventhandler = _eventhandler;

	//_event.addCallback(SDL_KEYDOWN,keyboard_callback,&_gamedata);
	//_event.addCallback(SDL_KEYUP,keyboard_callback,&_gamedata);
	//_event.addCallback(SDL_MOUSEMOTION,mouse_callback,&_gamedata);

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btAxisSweep3({0.0f,0.0f,0.0f},{500.0f,500.0f,500.0f});
	solver = new btSequentialImpulseConstraintSolver;
	
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	createObjectFromXML(dynamicsWorld,"assets/objects/xmliron.xml");

	//createObject(dynamicsWorld,loadObjectFromVertices(9,12,boxvertices,10.0f,-1,1,{0.0f,1.0f,0.0f},1,false),"assets/shaders/shader",DEFAULT,0.0f,{0.0f,15.0f,0.0f});
	//createObject(dynamicsWorld,loadObjectFromFile("assets/objects/colosseum/colosseum.obj","assets/objects/colosseum/",{1.0f,1.0f,1.0f}),"assets/shaders/shader",GROUND,0.0f,{0.0f,0.0f,0.0f});
	//createObject(dynamicsWorld,loadObjectFromFile("assets/objects/newiron/ironman.obj","assets/objects/newiron/",{0.005f,0.005f,0.005f}),"assets/shaders/shader",PLAYER,200.0f,{2.0f,2.8f,0.0f});
	//createObject(dynamicsWorld,"assets/objects/ironman/ironman.obj","assets/shaders/shader",PLAYER,{0.02f,0.02f,0.02f},10.0f,{0.0f,20.0f,0.0f});
	//createObjectFromJSON(dynamicsWorld,"assets/objects/newiron.json");

	#ifdef DO_DEBUG_DRAW
	//{
		BulletDebugDrawer_OpenGL debugDraw;
		debugDraw.setupShader("assets/shaders/debug");
		debugDraw.setDebugMode(DO_DEBUG_DRAW);
		dynamicsWorld->setDebugDrawer(&debugDraw);
	//}
	#endif

	std::cout << "Initialized" << std::endl;

	return 1;
}