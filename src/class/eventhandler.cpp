#include "class/eventhandler.h"
/*
void mouse_callback(SDL_Event _event, arg _arg)
{
	gamedata *_gamedata = static_cast<gamedata*>(_arg);

	if (_gamedata->firstMouse)
    {
        _gamedata->lastX = _event.motion.x;
        _gamedata->lastY = _event.motion.y;
        _gamedata->firstMouse = false;
    }

    float xoffset = _gamedata->lastX - _event.motion.x;
    float yoffset = _event.motion.y - _gamedata->lastY;

    _gamedata->lastX = _event.motion.x;
    _gamedata->lastY = _event.motion.y;

    _gamedata->_camera.ProcessMouseMovement(_event.motion.xrel, _event.motion.yrel);
}

void quit_callback(SDL_Event _event,arg _arg)
{
	gamedata *_gamedata = static_cast<gamedata*>(_arg);

	_gamedata->quit = false;
}
*/
eventhandler::eventhandler (gamedata *_gamedataptr)
{
	_gamedata = _gamedataptr;
}

void eventhandler::keyboardEvent(QKeyEvent *event, bool down)
{
	int key = event->key();

	switch (key)
    {
        case Qt::Key_W:
            _gamedata->_camera.ProcessKeyboard(FORWARD, down);
        break;

        case Qt::Key_S:
            _gamedata->_camera.ProcessKeyboard(BACKWARD, down);
        break;

        case Qt::Key_A:
            _gamedata->_camera.ProcessKeyboard(LEFT, down);
        break;

        case Qt::Key_D:
            _gamedata->_camera.ProcessKeyboard(RIGHT, down);
        break;

        case Qt::Key_Q:
            _gamedata->_camera.ProcessKeyboard(UP, down);
        break;

        case Qt::Key_E:
            _gamedata->_camera.ProcessKeyboard(DOWN, down);
        break;
    };

	_gamedata->glcontext->update();
}

void eventhandler::mouseEvent(QMouseEvent *event)
{
	auto mpos = event->pos();
	
	if (_gamedata->firstMouse)
    {
        _gamedata->lastX = mpos.x();
        _gamedata->lastY = mpos.y();
        _gamedata->firstMouse = false;
    }

    float xoffset = _gamedata->lastX - mpos.x();
    float yoffset = mpos.y() - _gamedata->lastY;

    _gamedata->lastX = mpos.x();
    _gamedata->lastY = mpos.y();

	std::cout << mpos.x() << ' ' << mpos.y() << ' ' << xoffset << ' ' << yoffset << std::endl;
	_gamedata->_camera.ProcessMouseMovement(xoffset, yoffset);

	_gamedata->glcontext->update();
}

void eventhandler::quitEvent()
{
	_gamedata->gamequit();
}