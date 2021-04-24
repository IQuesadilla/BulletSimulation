#include "gamemain.h"

explicit WindowWidget::WindowWidget(QWidget *parent = 0):QWidget(parent)
{
	m_button = new QPushButton("My Button");
	m_button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
	connect(m_button, &QPushButton::released, this, &KeyboardCallback::handleButton);

	mainLayout.addWidget(m_button);
	setLayout(&mainLayout);

	setWindowTitle("small penis");
	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	show();
}

void WindowWidget::setGLContextM(QWidget *glcontext)
{
	mainLayout.addWidget(glcontext);
}

void WindowWidget::handleButton()
{
	setWindowTitle("big penis");
}

void WindowWidget::keyPressEvent(QKeyEvent *event) override
{
	_eventhandler->keyboardEvent(event);
}

GLWidget::GLWidget()
{
	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	show();

	paintGL();
}

void GLWidget::paintEvent(QPaintEvent *event) override
{

}

void GLWidget::initializeGL() override
{
	initializeOpenGLFunctions();
}

void GLWidget::paintGL() override
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

gamedata::~gamedata()
{
	gamekill();
}

int main(int argc, char **argv)
{
	gamedata _gamedata (argc,argv);
	//_gamedata.gameloop();
	return _gamedata.app->exec();
}