#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>

#include "camera.h"

int event_callback(void *userdata, SDL_Event *event);

class _event
{
public:
    _event();
    ~_event();

    struct {
        Camera *camera;

        bool firstMouse = true;
        int lastX, lastY;
        bool quit = false;
    } data;

    void keyboard_update(SDL_Keycode event, bool down);
    void click_update(Uint8 event, bool down);
    void mouse_update(SDL_MouseMotionEvent event);
    void window_update(SDL_WindowEvent event);
    void quit_update();

//private:
    void update(SDL_Event *event);

};

#endif