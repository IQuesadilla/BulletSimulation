#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
//#include <Bullet3OpenCL/BroadphaseCollision/b3GpuGridBroadphase.h>

#include "gameloop.h"
#include "camera/camera.h"
#include "shader/shader.h"
#include "sdlwrapper/window.h"
#include "sdlwrapper/glcontext.h"
#include "sdlwrapper/event.h"
#include "object.h"
#include "debug.h"
#include "eventhandler.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//#define DO_DEBUG_DRAW btIDebugDraw::DebugDrawModes::DBG_DrawAabb | btIDebugDraw::DebugDrawModes::DBG_DrawWireframe

std::vector<btScalar> boxvertices = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

btTransform getTransform(btRigidBody *body)
{
	return body->getWorldTransform();
}

btTransform getTransform(btCollisionObject* obj)
{
	btRigidBody *body = btRigidBody::upcast(obj);
	btTransform trans;
	if (body && body->getMotionState())
		body->getMotionState()->getWorldTransform(trans);
	else
		trans = obj->getWorldTransform();
	return trans;
}

int gameloop(int argc, char **argv)
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
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	Camera camera(glm::vec3(0.0f,30.0f,5.0f));
	camera.setViewSize(WINDOW_WIDTH,WINDOW_HEIGHT);

	//_event Event;
	//Event.data.camera = &camera;
	//SDL_AddEventWatch(event_callback,&Event);

	event _event;
	eventdata _eventdata;
	_event.addCallback(SDL_KEYDOWN,keyboard_callback,&_eventdata);
	_event.addCallback(SDL_KEYUP,keyboard_callback,&_eventdata);
	_event.addCallback(SDL_MOUSEMOTION,mouse_callback,&_eventdata);

	btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface *overlappingPairCache = new btAxisSweep3({0.0f,0.0f,0.0f},{500.0f,500.0f,500.0f});
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;
	
	btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	//createObject(dynamicsWorld,loadObjectFromVertices(9,12,boxvertices,10.0f,-1,1,{0.0f,1.0f,0.0f},1,false),"assets/shaders/shader",DEFAULT,0.0f,{0.0f,15.0f,0.0f});
	//createObject(dynamicsWorld,loadObjectFromFile("assets/objects/colosseum/colosseum.obj","assets/objects/colosseum/",{1.0f,1.0f,1.0f}),"assets/shaders/shader",GROUND,0.0f,{0.0f,0.0f,0.0f});
	//createObject(dynamicsWorld,loadObjectFromFile("assets/objects/newiron/ironman.obj","assets/objects/newiron/",{0.005f,0.005f,0.005f}),"assets/shaders/shader",PLAYER,200.0f,{2.0f,2.8f,0.0f});
	//createObject(dynamicsWorld,"assets/objects/ironman/ironman.obj","assets/shaders/shader",PLAYER,{0.02f,0.02f,0.02f},10.0f,{0.0f,20.0f,0.0f});
	//createObjectFromJSON(dynamicsWorld,"assets/objects/newiron.json");
	createObjectFromXML(dynamicsWorld,"assets/objects/xmliron.xml");

	#ifdef DO_DEBUG_DRAW
	//{
		BulletDebugDrawer_OpenGL debugDraw;
		debugDraw.setupShader("assets/shaders/debug");
		debugDraw.setDebugMode(DO_DEBUG_DRAW);
		dynamicsWorld->setDebugDrawer(&debugDraw);
	//}
	#endif

	float deltaTime, lastFrame;
	bool reset = false;
	int fps, lastsec, fc;
	while (!Event.data.quit)
	{
		dynamicsWorld->stepSimulation(1.0f / 60.0f, 10, 1.0f / 60.0f);

		glClearColor (0.2f,0.2f,0.2f,1.0f);
		glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		{
			float currentFrame = (float)SDL_GetTicks()/1000.0f;
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (int(currentFrame) > lastsec)
			{
				lastsec = int(currentFrame);
				fps = fc; fc = 0;
				SDL_Log("%d",fps);
			}
			fc++;

			SDL_Event event;
			while (SDL_PollEvent (&event));
			camera.InputUpdate(deltaTime);
		}
		
		for (int j = 0; j < dynamicsWorld->getNumCollisionObjects(); j++)
		{
			btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[j];
			btCollisionShape *shape = obj->getCollisionShape();
			shapeobject *object = (shapeobject*)shape->getUserPointer();
			// if (object->type = "PLAYER")
			// 	{
					// btTransform trans = getTransform(obj);
					// camera.Position.x = trans.getOrigin().getX();
					// camera.Position.y = trans.getOrigin().getY() + 1.0f;
					// camera.Position.z = trans.getOrigin().getZ();

					// btRigidBody *body = btRigidBody::upcast(obj);
					// btTransform trans = obj->getWorldTransform();
					// btQuaternion quat = trans.getRotation();
					// btScalar yaw,pitch,roll;
					// quat.getEulerZYX(yaw,pitch,roll);
					// quat.setEuler(yaw,0.0f,0.0f);
					// trans.setRotation(quat);
					// obj->setWorldTransform(trans);

					// glm::vec3 rot = glm::eulerAngles(glm::quat(trans.getRotation().getW(),
					// 										trans.getRotation().getX(),
					// 										trans.getRotation().getY(),
					// 										trans.getRotation().getZ()));
					// camera.Pitch = rot.x;
					// camera.Yaw = rot.y;

					// object->shader.setVec3("objectColor", 0.5f, 0.5f, 0.5f);
					// object->shader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
					// object->shader.setVec3("lightPos",camera.Position);
					// object->shader.setVec3("viewPos",camera.Position);				
			// 	}
			// 	break;

			// 	case GROUND:

			// 	break;
			// }

			object->shader->use();
			object->shader->setVec3("objectColor", 0.0f, 0.0f, 0.5f);
			object->shader->setVec3("lightColor",  0.0f, 0.0f, 1.0f);
			object->shader->setVec3("lightPos",camera.Position);
			object->shader->setVec3("viewPos",camera.Position);	
		}

		// {
		// 	btTransform trans = getTransform(player);
		// 	camera.Position.x = trans.getOrigin().getX();
		// 	camera.Position.y = trans.getOrigin().getY() + 1.0f;
		// 	camera.Position.z = trans.getOrigin().getZ();

		// 	camera.Rotation = camera.Rotation * glm::toMat4(glm::quat(trans.getRotation().getW(),
		// 																trans.getRotation().getX(),
		// 																trans.getRotation().getY(),
		// 																trans.getRotation().getZ()));
		// }

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), camera.viewsizex / camera.viewsizey, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		dynamicsWorld->updateAabbs();
		//SDL_Log("%d ",dynamicsWorld->getNumCollisionObjects());
		for (int j = 0; j < dynamicsWorld->getNumCollisionObjects(); j++)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btTransform trans = getTransform(obj);

			btCollisionShape *shape = obj->getCollisionShape();
			shapeobject *object = (shapeobject*)shape->getUserPointer();
			{
				if (reset)
					obj->getWorldTransform().setOrigin(object->resettrans.getOrigin());
				object->shader->use();
				object->update();

				object->shader->setMat4("projection", projection);
				object->shader->setMat4("view",view);

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::scale(model, glm::vec3(shape->getLocalScaling().getX(),
				 									shape->getLocalScaling().getY(),
				 									shape->getLocalScaling().getZ()));
				model = glm::translate(model, glm::vec3(trans.getOrigin().getX() / shape->getLocalScaling().getX(),
														trans.getOrigin().getY() / shape->getLocalScaling().getY(),
														trans.getOrigin().getZ() / shape->getLocalScaling().getZ()));
				model = model * glm::toMat4(glm::quat(trans.getRotation().getW(),
													trans.getRotation().getX(),
													trans.getRotation().getY(),
													trans.getRotation().getZ()));
				//glm::mat4 model = glm::mat4(1.0f);;
				//trans.getOpenGLMatrix(&model[0][0]);
				object->shader->setMat4("model",model);

				glBindVertexArray(object->VAO);
				glDrawArrays (GL_TRIANGLES, 0, object->vertexcount);
			}

			//SDL_Log("world pos object %d = %f,%f,%f", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}

		#ifdef DO_DEBUG_DRAW
		dynamicsWorld->debugDrawWorld();
		//BulletDebugDrawer_OpenGL *debugDraw = ((BulletDebugDrawer_OpenGL*)dynamicsWorld->getDebugDrawer());
		debugDraw.shader.use();
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), camera.viewsizex / camera.viewsizey, 0.1f, 1000.0f);
		//glm::mat4 view = camera.GetViewMatrix();
		debugDraw.SetMatrices(view,projection,glm::mat3(1.0f));
		debugDraw.updateDraw();
		#endif

		if (reset) reset = false;
		SDL_GL_SwapWindow(window);
	}

	return 0;
}


int main(int argc, char **argv)
{
	return gameloop(argc, argv);
}
