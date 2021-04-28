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

	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	show();

	//paintGL();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
	_gamedata->gameloop(event);
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
}

void GLWidget::paintGL()
{
	//glClearColor(1.0f,1.0f,1.0f,1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	std::cout << "paintGL" << std::endl;
}

void GLWidget::resizeGL(int w, int h)
{
	std::cout << "resizeGL " << w << ' ' << h << std::endl;
}

gamedata::~gamedata()
{
	
}

int main(int argc, char **argv)
{
	gamedata _gamedata (argc,argv);
	return _gamedata.app->exec();
}