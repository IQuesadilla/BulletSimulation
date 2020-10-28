#pragma once

#define GL_GLEXT_PROTOTYPES

#include <LinearMath/btIDebugDraw.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.h"

// Helper class; draws the world as seen by Bullet.
// This is very handy to see it Bullet's world matches yours
// How to use this class :
// Declare an instance of the class :
// 
// dynamicsWorld->setDebugDrawer(&mydebugdrawer);
// Each frame, call it :
// mydebugdrawer.SetMatrices(ViewMatrix, ProjectionMatrix);
// dynamicsWorld->debugDrawWorld();
class BulletDebugDrawer_OpenGL : public btIDebugDraw {
public:
    GLuint VBO, VAO;
    _shader shader;
	std::vector<float> points;
    void setupShader(std::string path)
    {
        shader.load(path + ".vs", path + ".fs");
		//glGenBuffers(1, &VBO);
		//glGenVertexArrays(1, &VAO);
    }

	void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix, glm::mat4 pModelMatrix) 
	{
        shader.setMat4("projection",pProjectionMatrix);
        shader.setMat4("view",pViewMatrix);
		shader.setMat4("model",pModelMatrix);
	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) 
	{
		points.push_back(from.x());
		points.push_back(from.y());
		points.push_back(from.z());
		points.push_back(color.x());
		points.push_back(color.y());
		points.push_back(color.z());

		points.push_back(to.x());
		points.push_back(to.y());
		points.push_back(to.z());
		points.push_back(color.x());
		points.push_back(color.y());
		points.push_back(color.z());
	}
	virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {}
	virtual void reportErrorWarning(const char *) {}
	virtual void draw3dText(const btVector3 &, const char *) {}
	virtual void setDebugMode(int p) {
		m = p;
	}
	void updateDraw()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(points[0]), points.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, points.size());
		glBindVertexArray(0);
		// for (int j = 0; j < points.size(); j++)
		// 	std::cout << points[j] << std::endl;
		points.erase(points.begin(),points.end());
	}
	int getDebugMode(void) const { return m; }
	int m = DBG_NoDebug;
};