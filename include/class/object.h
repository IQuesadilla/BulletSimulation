#pragma once
#define GL_GLEXT_PROTOTYPES

#include "GL/gl.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <tiny_obj_loader.h>
#include <utility>
#include <map>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <json/json.h>

#include "shader.h"

struct shapeobject
{
	GLuint VAO;
	GLuint VBO;
	std::vector<float> vertices;
	_shader shader;

	btTransform resettrans;
	std::string type;

	void (*update)();
};
btCollisionShape* loadObjectFromVertices(int width, int height, std::vector<btScalar> data, btScalar scale, btScalar min, btScalar max, btVector3 color, int up, bool flip);
std::vector<btCollisionShape*> loadObjectFromFile(std::string objpath, std::string mtlpath, btVector3 scale);
void createObjectFromJSON(btDynamicsWorld *dynamicsWorld, std::string path);

void createObject(btDynamicsWorld *dynamicsWorld, std::vector<btCollisionShape*> shape, std::string shaderpath, std::string type, btScalar mass, btVector3 origin, btQuaternion rotation = {0.0f,0.0f,0.0f,1.0f});
