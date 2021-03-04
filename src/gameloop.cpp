#define GL_GLEXT_PROTOTYPES
#define GLM_ENABLE_EXPERIMENTAL
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

int gameloop(gamedata &_gamedata)
{
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

	float deltaTime, lastFrame;
	bool reset = false;
	int fps, lastsec, fc;
	while (!_gamedata.quit)
	{
		_gamedata.dynamicsWorld->stepSimulation(1.0f / 60.0f, 10, 1.0f / 60.0f);

		_gamedata._event.updateCallbacks();

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

			_gamedata._camera.InputUpdate(deltaTime);
		}
		
		for (int j = 0; j < _gamedata.dynamicsWorld->getNumCollisionObjects(); j++)
		{
			btCollisionObject *obj = _gamedata.dynamicsWorld->getCollisionObjectArray()[j];
			btCollisionShape *shape = obj->getCollisionShape();
			shapeobject *object = (shapeobject*)shape->getUserPointer();

			object->shader->use();
			object->shader->setVec3("objectColor", 0.0f, 0.0f, 0.5f);
			object->shader->setVec3("lightColor",  0.0f, 0.0f, 1.0f);
			object->shader->setVec3("lightPos",_gamedata._camera.Position);
			object->shader->setVec3("viewPos",_gamedata._camera.Position);	
		}

		glm::mat4 projection = glm::perspective(glm::radians(_gamedata._camera.Zoom), _gamedata._camera.viewsizex / _gamedata._camera.viewsizey, 0.1f, 1000.0f);
		glm::mat4 view = _gamedata._camera.GetViewMatrix();

		_gamedata.dynamicsWorld->updateAabbs();
		//SDL_Log("%d ",dynamicsWorld->getNumCollisionObjects());
		for (int j = 0; j < _gamedata.dynamicsWorld->getNumCollisionObjects(); j++)
		{
			btCollisionObject* obj = _gamedata.dynamicsWorld->getCollisionObjectArray()[j];
			update_object_graphics(obj,projection,view,reset);
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
		_gamedata._glcontext.update();
	}

	return 0;
}
