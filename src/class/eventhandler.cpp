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

int event_callback(void *userdata, SDL_Event *event)
{
    ((_event*)userdata)->update(event);
    return 0;
};

_event::_event()
{ 

}

_event::~_event()
{

}

void _event::update(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN:
            keyboard_update(event->key.keysym.sym, true);
        break;

        case SDL_KEYUP:
            keyboard_update(event->key.keysym.sym, false);
        break;

        case SDL_MOUSEBUTTONDOWN:
            click_update(event->button.button, true);
        break;

        case SDL_MOUSEBUTTONUP:
            click_update(event->button.button, false);
        break;

        case SDL_MOUSEMOTION:
            mouse_update(event->motion);
        break;

        case SDL_WINDOWEVENT:
            window_update(event->window);
        break;

        case SDL_QUIT:
            quit_update();
        break;
    };
}

void _event::keyboard_update(SDL_Keycode event, bool down)
{
    switch (event)
    {
        case SDLK_w:
            data.camera->ProcessKeyboard(FORWARD, down);
        break;

        case SDLK_s:
            data.camera->ProcessKeyboard(BACKWARD, down);
        break;

        case SDLK_a:
            data.camera->ProcessKeyboard(LEFT, down);
        break;

        case SDLK_d:
            data.camera->ProcessKeyboard(RIGHT, down);
        break;

        case SDLK_q:
            data.camera->ProcessKeyboard(UP, down);
        break;

        case SDLK_e:
            data.camera->ProcessKeyboard(DOWN, down);
        break;

        case SDLK_ESCAPE:
            quit_update();
        break;
    };
}

void _event::click_update(Uint8 event, bool down){}

void _event::mouse_update(SDL_MouseMotionEvent event)
{
    if (data.firstMouse)
    {
        data.lastX = event.x;
        data.lastY = event.y;
        data.firstMouse = false;
    }

    float xoffset = data.lastX - event.x;
    float yoffset = event.y - data.lastY;

    data.lastX = event.x;
    data.lastY = event.y;

    data.camera->ProcessMouseMovement(event.xrel, event.yrel);
}

void _event::window_update(SDL_WindowEvent event)
{
    switch (event.event)
    {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            data.camera->setViewSize(event.data1,event.data2);
        break;
    };
}

void _event::quit_update()
{
    SDL_Log("%s", "Program killed");
    data.quit = true;
}