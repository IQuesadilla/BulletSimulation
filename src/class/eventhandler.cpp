#include "eventhandler.h"

void keydown_callback (SDL_Event _event, arg _arg)
{
	eventdata *_eventdata = static_cast<eventdata*>(_arg);

	bool down;
	if (_event.type == SDL_KEYDOWN)
		down = true;
	else
		down = false;
	
	switch (_event.key.keysym.sym)
    {
        case SDLK_w:
            _eventdata->camera->ProcessKeyboard(FORWARD, down);
        break;

        case SDLK_s:
            _eventdata->camera->ProcessKeyboard(BACKWARD, down);
        break;

        case SDLK_a:
            _eventdata->camera->ProcessKeyboard(LEFT, down);
        break;

        case SDLK_d:
            _eventdata->camera->ProcessKeyboard(RIGHT, down);
        break;

        case SDLK_q:
            _eventdata->camera->ProcessKeyboard(UP, down);
        break;

        case SDLK_e:
            _eventdata->camera->ProcessKeyboard(DOWN, down);
        break;

        case SDLK_ESCAPE:
            quit_callback(_event,_arg);
        break;
    };
}

void mouse_callback(SDL_Event _event, arg _arg)
{
	eventdata *_eventdata = static_cast<eventdata*>(_arg);

	if (_eventdata->firstMouse)
    {
        _eventdata->lastX = _event.motion.x;
        _eventdata->lastY = _event.motion.y;
        _eventdata->firstMouse = false;
    }

    float xoffset = _eventdata->lastX - _event.motion.x;
    float yoffset = _event.motion.y - _eventdata->lastY;

    _eventdata->lastX = _event.motion.x;
    _eventdata->lastY = _event.motion.y;

    _eventdata->camera->ProcessMouseMovement(_event.motion.xrel, _event.motion.yrel);
}

void quit_callback(SDL_Event _event,arg _arg)
{
	eventdata *_eventdata = static_cast<eventdata*>(_arg);

	_eventdata->quit = false;
}