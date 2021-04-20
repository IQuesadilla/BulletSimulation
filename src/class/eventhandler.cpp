#include "class/eventhandler.h"

void keyboard_callback (SDL_Event _event, arg _arg)
{
	gamedata *_gamedata = static_cast<gamedata*>(_arg);

	bool down;
	if (_event.type == SDL_KEYDOWN)
		down = true;
	else
		down = false;
	
	switch (_event.key.keysym.sym)
    {
        case SDLK_w:
            _gamedata->_camera.ProcessKeyboard(FORWARD, down);
        break;

        case SDLK_s:
            _gamedata->_camera.ProcessKeyboard(BACKWARD, down);
        break;

        case SDLK_a:
            _gamedata->_camera.ProcessKeyboard(LEFT, down);
        break;

        case SDLK_d:
            _gamedata->_camera.ProcessKeyboard(RIGHT, down);
        break;

        case SDLK_q:
            _gamedata->_camera.ProcessKeyboard(UP, down);
        break;

        case SDLK_e:
            _gamedata->_camera.ProcessKeyboard(DOWN, down);
        break;

        case SDLK_ESCAPE:
            quit_callback(_event,_arg);
        break;
    };
}

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

void keyboardEvent(QKeyEvent *event)
{
	
}