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
	_eventhandler->keyboardEvent(event);
}

GLWidget::GLWidget()
{
	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	show();

	paintGL();
}

void GLWidget::paintEvent(QPaintEvent *event)
{

}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
}

void GLWidget::paintGL()
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

gamedata::~gamedata()
{
	
}

int main(int argc, char **argv)
{
	gamedata _gamedata (argc,argv);
	//_gamedata.gameloop();
	return _gamedata.app->exec();
}