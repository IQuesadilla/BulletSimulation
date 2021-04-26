#include "gamemain.h"

void WindowWidget::setGLContextM(QWidget *glcontext)
{
	mainLayout.addWidget(glcontext);
}

void WindowWidget::handleButton()
{
	setWindowTitle("big penis");
}

void WindowWidget::keyPressEvent(QKeyEvent *event)
{
	_eventhandler->keyboardEvent(event,true);
}

void WindowWidget::keyReleaseEvent(QKeyEvent *event)
{
	_eventhandler->keyboardEvent(event,false);
}

GLWidget::GLWidget()
{
	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	show();

	paintGL();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
	std::cout << "paintEvent" << std::endl;
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

gamedata::~gamedata()
{
	
}

int main(int argc, char **argv)
{
	gamedata _gamedata (argc,argv);
	return _gamedata.app->exec();
}