#pragma once

#define GL_GLEXT_PROTOTYPES
#define GLM_ENABLE_EXPERIMENTAL

#include "camera/camera.h"

#include "class/eventhandler.h"
#include "class/object.h"

#include "bullet/BulletCollision/btBulletCollisionCommon.h"
#include "bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

#include <QtCore/QtCore>
#include <QtWidgets/QApplication>
#include <QtWidgets/QtWidgets>
#include <QtCore/QTextStream>
#include <QtOpenGL/QGL>
#include <iostream>


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct eventhandler;

class WindowWidget : public QWidget
{
public:
    explicit WindowWidget(QWidget *parent = 0):QWidget(parent)
	{
		setLayout(&mainLayout);

		setWindowTitle("small penis");
		resize(WINDOW_WIDTH, WINDOW_HEIGHT);
		show();
	};
    void setGLContextM(QWidget *glcontext);

	eventhandler *_eventhandler;

protected:
    void handleButton();
    void keyPressEvent(QKeyEvent *event) override;

    QHBoxLayout mainLayout;
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void initializeGL() override;
    void paintGL() override;
};

struct gamedata
{
	gamedata(int argc, char **argv){gameinit(argc,argv);};
	~gamedata();

	int gameinit(int argc, char **argv);
	int gameloop();

	eventhandler *_eventhandler;
	Camera _camera;

	QApplication *app;
	WindowWidget *window;
	GLWidget *glcontext;

	btDefaultCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btBroadphaseInterface *overlappingPairCache;
	btSequentialImpulseConstraintSolver *solver;
	btDiscreteDynamicsWorld *dynamicsWorld;

	bool firstMouse = true;
	int lastX, lastY;
	bool quit = false;

	float deltaTime, lastFrame;
	bool reset = false;
	int fps, lastsec, fc;
};