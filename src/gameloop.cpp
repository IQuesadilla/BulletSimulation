#define GL_GLEXT_PROTOTYPES
#define GLM_ENABLE_EXPERIMENTAL
//#include <SDL.h>
//#include <GL/gl.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
//#include <Bullet3OpenCL/BroadphaseCollision/b3GpuGridBroadphase.h>

#include "gamemain.h"
#include "camera/camera.h"
#include "shader/shader.h"
//#include "sdlwrapper/window.h"
//#include "sdlwrapper/glcontext.h"
//#include "sdlwrapper/event.h"
#include "class/object.h"
#include "class/debug.h"
#include "class/eventhandler.h"

//#define DO_DEBUG_DRAW btIDebugDraw::DebugDrawModes::DBG_DrawAabb | btIDebugDraw::DebugDrawModes::DBG_DrawWireframe

int gamedata::gameloop()
{
	//std::cout << event << std::endl;
	//glcontext->makeCurrent();
	dynamicsWorld->stepSimulation(1.0f / 60.0f, 10, 1.0f / 60.0f);

	//_event.updateCallbacks();

	glClearColor (1.0f,0.0f,0.0f,1.0f);
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	
	float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(timer.now() - start).count() / 1000.0f;
	start = timer.now();

	window->setWindowTitle(QString::fromStdString(std::to_string(int(1.0f / deltaTime))));

	_camera.InputUpdate(deltaTime);
	
	for (int j = 0; j < dynamicsWorld->getNumCollisionObjects(); j++)
	{
		btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[j];
		btCollisionShape *shape = obj->getCollisionShape();
		shapeobject *object = (shapeobject*)shape->getUserPointer();

		object->shader->use();
		object->shader->setVec3("objectColor", 0.0f, 0.0f, 0.5f);
		object->shader->setVec3("lightColor",  0.0f, 0.0f, 1.0f);
		object->shader->setVec3("lightPos",_camera.Position);
		object->shader->setVec3("viewPos",_camera.Position);	
	}
	//window->update();
/*
	#ifdef DO_DEBUG_DRAW
	dynamicsWorld->debugDrawWorld();
	//BulletDebugDrawer_OpenGL *debugDraw = ((BulletDebugDrawer_OpenGL*)dynamicsWorld->getDebugDrawer());
	debugDraw.shader.use();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), camera.viewsizex / camera.viewsizey, 0.1f, 1000.0f);
	//glm::mat4 view = camera.GetViewMatrix();
	debugDraw.SetMatrices(view,projection,glm::mat3(1.0f));
	debugDraw.updateDraw();
	#endif
*/
	//if (reset) reset = false;
	//_glcontext.update();

	return 0;
}

int gamedata::gamedraw()
{
	std::cout << "gamedraw" << std::endl;
	dynamicsWorld->updateAabbs();
	for (int j = 0; j < dynamicsWorld->getNumCollisionObjects(); j++)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		//update_object_graphics(obj,projection,view,reset);
		btTransform trans = getTransform(obj);

		btCollisionShape *shape = obj->getCollisionShape();
		shapeobject *object = (shapeobject*)shape->getUserPointer();

		//if (reset)
		//	obj->getWorldTransform().setOrigin(object->resettrans.getOrigin());
		object->shader->use();
		object->update();

		glm::mat4 projection = _camera.GetProjectionMatrix();
		glm::mat4 view = _camera.GetViewMatrix();
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

		object->shader->setMat4("projection", projection);
		object->shader->setMat4("view",view);
		object->shader->setMat4("model",model);

		glBindVertexArray(object->VAO);
		glDrawArrays (GL_TRIANGLES, 0, object->vertexcount);

		//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}

	return 0;
}