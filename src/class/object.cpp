#include "object.h"

#define TRIANGLE_VERTEX_COUNT 9
#define TVC TRIANGLE_VERTEX_COUNT

// void printvert (void *vertices, int size)
// {
// 	for (int i = 0; i < size; i++)
// 		vertices
// }

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

class tricallback : public btTriangleCallback
{
public:
	std::vector<btVector3> vertices;
	int loc = 0;
	void processTriangle(btVector3* triangle, int partId, int triangleIndex)
	{
		//int loc = (partId-6)+(triangleIndex-5);
		std::cout << loc << std::endl;
		if (loc+1 >= vertices.size())
			vertices.resize(loc+1);
		vertices.at(loc) = *triangle; loc++;
		//indices.push_back((partId)*(triangleIndex-6));
	}
};

template<typename K, typename V>
void print_map(std::map<K,V> const &m)
{
    for (auto const& pair: m) {
        std::cout << "{" << pair.first << ":" << pair.second << "}\n";
    }
}

void scaleVertices(std::vector<btScalar> *vertices, btVector3 scale)
{
	for (int j = 0; j < vertices->size(); j+=9)
		for (int i = 0; i < 3; i++)
			vertices->at(j+i) *= scale[i];
}

std::vector<btScalar> loadObjFile(std::string path, std::string mtlpath)
{
	std::vector<std::vector<btScalar>> vertices;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn, err;
	bool success = false;
	success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), mtlpath.c_str());

	if (!warn.empty())
		SDL_LogWarn(SDL_LOG_CATEGORY_VIDEO,"%s",warn.c_str());
	if (!err.empty())
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO,"%s",warn.c_str());
	if (!success) std::cout << path << std::endl;

	std::cout << shapes.size() << ' ' << materials.size() << std::endl;
	for (int s = 0; s < shapes.size(); s++)
	{
		vertices.push_back(std::vector<btScalar>());
		int index_offset = 0;
		//std::cout << shapes[s].mesh.num_face_vertices.size() << std::endl;
		for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			btVector3 color = btVector3(1.0f,1.0f,1.0f);
			int fv = shapes[s].mesh.num_face_vertices[f];
			for (int v = 0; v < fv; v++)
			{
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset+v];
				vertices.at(s).push_back(attrib.vertices[3*idx.vertex_index+0]);
				vertices.at(s).push_back(attrib.vertices[3*idx.vertex_index+1]);
				vertices.at(s).push_back(attrib.vertices[3*idx.vertex_index+2]);
				vertices.at(s).push_back(attrib.normals[3*idx.normal_index+0]);
				vertices.at(s).push_back(attrib.normals[3*idx.normal_index+1]);
				vertices.at(s).push_back(attrib.normals[3*idx.normal_index+2]);
				if (materials.size())
				{
					vertices.at(s).push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[0]); 
					vertices.at(s).push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[1]);
					vertices.at(s).push_back(materials[shapes[s].mesh.material_ids[f]].diffuse[2]);
					print_map(materials[shapes[s].mesh.material_ids[f]].unknown_parameter);
				}
				else
				{
					vertices.at(s).push_back(color[0]);
					vertices.at(s).push_back(color[1]);
					vertices.at(s).push_back(color[2]);
				}
			}
			index_offset += fv;
		}
	}
	return vertices[0];
}

std::vector<btCollisionShape*> loadObjectFromFile(std::string objpath, std::string mtlpath, btVector3 scale)
{
	std::vector<btCollisionShape*> shapevector;
	std::vector<std::vector<btScalar> > vertices;// = loadObjFile(objpath,mtlpath);

	int s = 0;
	for (auto& x : vertices)
	{
		scaleVertices(&x,scale);

		btTriangleMesh *mesh = new btTriangleMesh();
		for (int j = 0; j < x.size()/TVC; j+=3*4)
			mesh->addTriangle(	btVector3(x[0+(j*TVC)],x[1+(j*TVC)],x[2+(j*TVC)]),
						btVector3(x[9+(j*TVC)],x[10+(j*TVC)],x[11+(j*TVC)]),
						btVector3(x[18+(j*TVC)],x[19+(j*TVC)],x[20+(j*TVC)]));

		s += x.size();

		btCollisionShape *shape = new btConvexTriangleMeshShape(mesh,true);
		shapeobject *tempshape = new shapeobject;
		shape->setUserIndex(true);
		shape->setUserPointer(tempshape);
		tempshape->vertexcount = x.size();
		shapevector.push_back(shape);
		std::cout << x.size() << std::endl;
	}
	std::cout << s << std::endl;
	return shapevector;
}

btCollisionShape* loadObjectHeightfield(int width, int height, std::vector<btScalar> data, btScalar scale, btScalar min, btScalar max, btVector3 color, int up, bool flip)
{/*
	btHeightfieldTerrainShape *shape = new btHeightfieldTerrainShape(width,height,data.data(),scale,min,max,up,PHY_FLOAT,flip);
	//btTriangleInfoMap *map = shape->getTriangleInfoMap(); shape->setTriangleInfoMap();
	//btTriangleMeshShape *mesh = shape;
	//PHY_ScalarType vtype, itype;
	//btScalar *vertices, *indices;
	//int vcount, facecount, vstride, istride;
	//std::cout << "here" << std::endl;
	//btCollisionShape *colshape = new btBoxShape({1,1,1});
	shapeobject *tempshape = new shapeobject;
	shape->setUserPointer(tempshape);
	//std::cout << shape->getTriangleInfoMap()->size() << std::endl;

	tricallback *callback = new tricallback;
	btVector3 aabbmin = {0,0,0}, aabbmax = {1.0f,1.0f,1.0f};
	shape->processAllTriangles(callback,aabbmin,aabbmax);

	int x = 0, y = 0;
	//mesh->getLockedVertexIndexBase((unsigned char**)&vertices, vcount, vtype, vstride, (unsigned char**)&indices, istride, facecount, itype);
	std::cout << callback->vertices.size() << std::endl;
	for (int j = 0; j < callback->vertices.size(); j++)
	{
		//std::cout << j << std::endl;
	 	//btTriangleInfo *tri = map->find(map->findIndex(j));
		// x++;
		// if (x > width)
		// {
		// 	x = 0; y++;
		// }
		//btVector3 vert; shape->getVertex(x,y,vert);
			
		std::cout << j << ' ' << ' ' << callback->vertices[j][1] << std::endl;
		tempshape->vertices.push_back(callback->vertices[j][0]);
		tempshape->vertices.push_back(callback->vertices[j][1]);
		tempshape->vertices.push_back(callback->vertices[j][2]);
		tempshape->vertices.push_back(0);
		tempshape->vertices.push_back(0);
		tempshape->vertices.push_back(0);
		tempshape->vertices.push_back(color[0]);
		tempshape->vertices.push_back(color[1]);
		tempshape->vertices.push_back(color[2]);
	}
	//std::cout << facecount << std::endl;

	std::vector<btCollisionShape*> shapevector;
	shapevector.push_back(shape);
	return shape;
*/}

void dupdate(){};
void createObjectFromJSON(btDynamicsWorld *dynamicsWorld, std::string path)
{
	Json::Value root;
	std::ifstream stream;
	stream.open(path);
	stream >> root;

	shapeobject *tempshape = new shapeobject;
	btCollisionShape* shape;

	std::vector<btScalar> graphicsvertices;

	std::string type = root["type"].asString();

	if (type == "file")
	{
		std::string modelpath = root["modelpath"].asString();
		graphicsvertices = loadObjFile(modelpath+"/model.obj",modelpath);
		std::vector<btScalar> bodyvertices = loadObjFile(modelpath+"/body.obj","");

		btTriangleMesh *mesh = new btTriangleMesh();
		for (int j = 0; j < bodyvertices.size()/TVC; j+=3*4)
			mesh->addTriangle(	btVector3(bodyvertices[0+(j*TVC)],bodyvertices[1+(j*TVC)],bodyvertices[2+(j*TVC)]),
								btVector3(bodyvertices[3+(j*TVC)],bodyvertices[4+(j*TVC)],bodyvertices[5+(j*TVC)]),
								btVector3(bodyvertices[6+(j*TVC)],bodyvertices[7+(j*TVC)],bodyvertices[8+(j*TVC)]));

		shape = new btConvexTriangleMeshShape(mesh,true);

		tempshape->vertexcount = graphicsvertices.size();
		std::cout << "SIZE: " << graphicsvertices.size() << std::endl;
	}
	else if (type == "box")
	{
		shape = new btBoxShape(btVector3(	root["boxHalfExtents"].get("x",1.0f).asFloat(),
											root["boxHalfExtents"].get("y",1.0f).asFloat(),
											root["boxHalfExtents"].get("z",1.0f).asFloat()));
	}

	shape->setUserPointer(tempshape);

	if (root.isMember("scale")) shape->setLocalScaling(btVector3(	root["scale"].get("x",1.0f).asFloat(),
																	root["scale"].get("y",1.0f).asFloat(),
																	root["scale"].get("z",1.0f).asFloat()));

	btTransform trans;
	trans.setIdentity();
	trans.setRotation(btQuaternion(	root["rotation"].get("yaw",0.0f).asFloat(),
									root["rotation"].get("pitch",0.0f).asFloat(),
									root["rotation"].get("roll",0.0f).asFloat()));
	trans.setOrigin(btVector3(	root["origin"].get("x",0.0f).asFloat(),
								root["origin"].get("y",0.0f).asFloat(),
								root["origin"].get("z",0.0f).asFloat()));

	tempshape->resettrans = trans;
	tempshape->update = dupdate;

	btScalar mass = root.get("mass",1.0f).asFloat();
	btVector3 inertia;
	shape->calculateLocalInertia(mass,inertia);

	btDefaultMotionState *motionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionState,shape,inertia);
	btRigidBody *body = new btRigidBody(rbInfo);

	if (root.isMember("friction")) body->setFriction(root["friction"].asFloat());
	if (root.isMember("gravity")) body->setGravity(btVector3(	root["gravity"]["x"].asFloat(),
																root["gravity"]["y"].asFloat(),
																root["gravity"]["z"].asFloat()));

	dynamicsWorld->addRigidBody(body);
	dynamicsWorld->updateSingleAabb(body);

	glGenVertexArrays(1, &tempshape->VAO);
	glGenBuffers(1, &tempshape->VBO);

	glBindVertexArray(tempshape->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, tempshape->VBO);
	glBufferData(GL_ARRAY_BUFFER, graphicsvertices.size()*sizeof(float), graphicsvertices.data(), GL_STATIC_DRAW);
		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	//SDL_Log("%s","Here");

	std::string shaderpath = root["shaderpath"].asString();
	tempshape->shader; tempshape->shader.reset(new _shader());
	tempshape->shader->load(shaderpath+".vs",shaderpath+".fs");
}

void createObjectFromXML(btDynamicsWorld *dynamicsWorld, std::string path)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());
	std::string xmlpath = doc.FirstChildElement("MODEL")->FirstChildElement("PATH")->GetText();
	daeloader dae;
	dae.import(xmlpath);
	
	shapeobject *tempshape = new shapeobject;
	btCollisionShape* shape;

	btTriangleMesh *mesh = new btTriangleMesh();
	for (int j = 0; j < dae.indices.size()/TVC; j+=3)
			mesh->addTriangle(	btVector3(dae.indices[0+(j*TVC)],dae.indices[1+(j*TVC)],dae.indices[2+(j*TVC)]),
								btVector3(dae.indices[3+(j*TVC)],dae.indices[4+(j*TVC)],dae.indices[5+(j*TVC)]),
								btVector3(dae.indices[6+(j*TVC)],dae.indices[7+(j*TVC)],dae.indices[8+(j*TVC)]));

	shape = new btConvexTriangleMeshShape(mesh,true);

	//tempshape->vertices = dae.indices;
	tempshape->vertexcount = dae.indices.size();

	shape->setUserPointer(tempshape);

//	if (root.isMember("scale")) shape->setLocalScaling(btVector3(	root["scale"].get("x",1.0f).asFloat(),
//																	root["scale"].get("y",1.0f).asFloat(),
//																	root["scale"].get("z",1.0f).asFloat()));

	btTransform trans;
	trans.setIdentity();
//	trans.setRotation(btQuaternion(	root["rotation"].get("yaw",0.0f).asFloat(),
//									root["rotation"].get("pitch",0.0f).asFloat(),
//									root["rotation"].get("roll",0.0f).asFloat()));
//	trans.setOrigin(btVector3(	root["origin"].get("x",0.0f).asFloat(),
//								root["origin"].get("y",0.0f).asFloat(),
//								root["origin"].get("z",0.0f).asFloat()));

	tempshape->resettrans = trans;
	tempshape->update = dupdate;

	btScalar mass = 0.0f;//root.get("mass",1.0f).asFloat();
	btVector3 inertia;
	shape->calculateLocalInertia(mass,inertia);

	btDefaultMotionState *motionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,motionState,shape,inertia);
	btRigidBody *body = new btRigidBody(rbInfo);

//	if (root.isMember("friction")) body->setFriction(root["friction"].asFloat());
//	if (root.isMember("gravity")) body->setGravity(btVector3(	root["gravity"]["x"].asFloat(),
//																root["gravity"]["y"].asFloat(),
//																root["gravity"]["z"].asFloat()));


	dynamicsWorld->addRigidBody(body);
	dynamicsWorld->updateSingleAabb(body);

	glGenVertexArrays(1, &tempshape->VAO);
	glGenBuffers(1, &tempshape->VBO);

	glBindVertexArray(tempshape->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, tempshape->VBO);
	glBufferData(GL_ARRAY_BUFFER, dae.indices.size()*sizeof(float), dae.indices.data(), GL_STATIC_DRAW);
		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
	//glEnableVertexAttribArray(2);

	//SDL_Log("%s","Here");

	std::string shaderpath = doc.FirstChildElement("SHADER")->FirstChildElement("PATH")->GetText();
	tempshape->shader; tempshape->shader.reset(new _shader());
	tempshape->shader->load(shaderpath+".vs",shaderpath+".fs");}

btCollisionShape *loadObject(btVector3 boxHalfExtents, btVector3 color)
{
	btBoxShape *shape = new btBoxShape(boxHalfExtents);
	return shape;
}

void createObject(btDynamicsWorld *dynamicsWorld, std::vector<btCollisionShape*> shapevector, std::vector<Point3D> vertices, std::string shaderpath, std::string type, btScalar mass, btVector3 origin, btQuaternion rotation)
{
	for (auto& shape : shapevector)
	{
		shapeobject *tempshape = (shapeobject*)shape->getUserPointer();
		btTransform trans;
		trans.setIdentity();
		trans.setRotation(rotation);
		trans.setOrigin(origin);
		tempshape->resettrans = trans;
		tempshape->type = type;

		tempshape->update = dupdate;
		
		btVector3 inertia;
		shape->calculateLocalInertia(mass,inertia);

		btDefaultMotionState *myMotionState = new btDefaultMotionState(trans);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,inertia);
		btRigidBody *body = new btRigidBody(rbInfo);
		body->setFriction(1000.0f);
		body->setSpinningFriction(1000.0f);
		body->setRollingFriction(1000.0f);
		body->setAngularFactor(0.0f);
		body->setActivationState(DISABLE_DEACTIVATION);

		dynamicsWorld->addRigidBody(body);
		dynamicsWorld->updateSingleAabb(body);

		glGenVertexArrays(1, &tempshape->VAO);
		glGenBuffers(1, &tempshape->VBO);

		glBindVertexArray(tempshape->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, tempshape->VBO);
		glBufferData(GL_ARRAY_BUFFER, tempshape->vertexcount*sizeof(float), vertices.data(), GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6*sizeof(float)));
		glEnableVertexAttribArray(2);

		tempshape->shader; tempshape->shader.reset(new _shader());
		tempshape->shader->load(shaderpath+".vs",shaderpath+".fs");
	}

	return;
}

void update_object_graphics(btCollisionObject* obj, glm::mat4 projection, glm::mat4 view, bool reset)
{
	btTransform trans = getTransform(obj);

	btCollisionShape *shape = obj->getCollisionShape();
	shapeobject *object = (shapeobject*)shape->getUserPointer();
	{
		if (reset)
			obj->getWorldTransform().setOrigin(object->resettrans.getOrigin());
		object->shader.use();
		object->update();

		object->shader.setMat4("projection", projection);
		object->shader.setMat4("view",view);

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
		object->shader.setMat4("model",model);

		glBindVertexArray(object->VAO);
		glDrawArrays (GL_TRIANGLES, 0, object->vertices.size());
	}
}