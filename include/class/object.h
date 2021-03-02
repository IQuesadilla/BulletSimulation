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
#include <tinyxml2.h>
#include <fstream>

#include "shader/shader.h"
#include "daeload/daeload.h"

struct shapeobject
{
	GLuint VAO;
	GLuint VBO;
	//std::vector<float> vertices;
	int vertexcount;
	std::shared_ptr<_shader> shader;

	btTransform resettrans;
	std::string type;

	void (*update)();
};
btCollisionShape* loadObjectFromVertices(int width, int height, std::vector<btScalar> data, btScalar scale, btScalar min, btScalar max, btVector3 color, int up, bool flip);
std::vector<btCollisionShape*> loadObjectFromFile(std::string objpath, std::string mtlpath, btVector3 scale);
void createObjectFromJSON(btDynamicsWorld *dynamicsWorld, std::string path);
void createObjectFromXML(btDynamicsWorld *dynamicsWorld, std::string path);

void createObject(btDynamicsWorld *dynamicsWorld, std::vector<btCollisionShape*> shapevector, std::vector<Point3D> vertices, std::string shaderpath, std::string type, btScalar mass, btVector3 origin, btQuaternion rotation = {0.0f,0.0f,0.0f,1.0f});
