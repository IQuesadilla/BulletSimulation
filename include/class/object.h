#pragma once
#define GL_GLEXT_PROTOTYPES
#define GLM_ENABLE_EXPERIMENTAL

#include "GL/gl.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#include <tiny_obj_loader.h>
#include <utility>
#include <map>
<<<<<<< HEAD
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
=======
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
>>>>>>> 0d4b352ebb84f23dcc806247364096017a90f18e
#include <json/json.h>
#include <tinyxml2.h>
#include <fstream>

#include "camera.h"
#include "shader.h"
#include "daeload.h"

struct shapeobject
{
	GLuint VAO;
	GLuint VBO;
	//std::vector<float> vertices;
	std::shared_ptr<_shader> shader;

	btTransform resettrans;
	std::string type;

	void (*update)();
};

btTransform getTransform(btRigidBody *body);
btTransform getTransform(btCollisionObject* obj);

btCollisionShape* loadObjectFromVertices(int width, int height, std::vector<btScalar> data, btScalar scale, btScalar min, btScalar max, btVector3 color, int up, bool flip);
std::vector<btCollisionShape*> loadObjectFromFile(std::string objpath, std::string mtlpath, btVector3 scale);
void createObjectFromJSON(btDynamicsWorld *dynamicsWorld, std::string path);
void createObjectFromXML(btDynamicsWorld *dynamicsWorld, std::string path);

void createObject(btDynamicsWorld *dynamicsWorld, std::vector<btCollisionShape*> shape, std::string shaderpath, std::string type, btScalar mass, btVector3 origin, btQuaternion rotation = {0.0f,0.0f,0.0f,1.0f});

void update_object_graphics(btCollisionObject* obj, glm::mat4 projection, glm::mat4 view, bool reset);