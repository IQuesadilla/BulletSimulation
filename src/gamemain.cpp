#include "gamemain.h"

void WindowWidget::setGLContextM(QWidget *glcontext)
{
	mainLayout.addWidget(glcontext);
}

void WindowWidget::keyPressEvent(QKeyEvent *event)
{
	_eventhandler->keyboardEvent(event,true);
}

void WindowWidget::keyReleaseEvent(QKeyEvent *event)
{
	_eventhandler->keyboardEvent(event,false);
}

void WindowWidget::mouseMoveEvent(QMouseEvent *event)
{
	_eventhandler->mouseEvent(event);
}

void WindowWidget::mousePressEvent(QMouseEvent *event)
{

}

void WindowWidget::mouseReleaseEvent(QMouseEvent *event)
{
	
}

GLWidget::GLWidget(gamedata *_gamedataptr)
{
	_gamedata = _gamedataptr;

	//paintGL();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
	_gamedata->gameloop(event);
	_gamedata->gamedraw();
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	show();
	std::cout << "GL INIT" << std::endl;
}

void GLWidget::paintGL()
{

}

void GLWidget::resizeGL(int w, int h)
{
	std::cout << "resizeGL " << w << ' ' << h << std::endl;
	resize(w,h);
	_gamedata->_camera.setViewSize(w,h);
}

gamedata::~gamedata()
{
	
}

int main(int argc, char **argv)
{
	gamedata _gamedata (argc,argv);
	return _gamedata.app->exec();
}