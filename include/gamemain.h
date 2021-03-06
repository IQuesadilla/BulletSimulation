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
struct gamedata;

class WindowWidget : public QWidget
{
public:
    WindowWidget();
    void setGLContextM(QWidget *glcontext);

	gamedata *_gamedata;

protected:
    void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

    QHBoxLayout mainLayout;
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLWidget(gamedata *_gamedataptr);

	gamedata *_gamedata;

protected:
    //void paintEvent(QPaintEvent *event) override;
    void initializeGL() override;
    void paintGL() override;
	void resizeGL(int w, int h) override;
};

struct gamedata
{
	gamedata(int argc, char **argv){gameinit(argc,argv);};
	~gamedata();

	int gameinit(int argc, char **argv);
	int gameloop();
	int gamedraw();
	int gamequit(){return 0;};

	eventhandler *_eventhandler;
	Camera _camera;

	QApplication *app;
	WindowWidget *window;
	GLWidget *glcontext;

	QLineEdit *textbox;

	btDefaultCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btBroadphaseInterface *overlappingPairCache;
	btSequentialImpulseConstraintSolver *solver;
	btDiscreteDynamicsWorld *dynamicsWorld;

	std::chrono::_V2::system_clock::time_point start;
	std::chrono::high_resolution_clock timer;

	bool firstMouse = true;
	int lastX, lastY;
	bool quit = false;
};